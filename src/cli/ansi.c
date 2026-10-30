#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* ansi.c: colour, terminal width and word wrapping for text output. */

#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "cli.h"

#define MIN_WIDTH 20
#define MAX_WIDTH 100
#define DEFAULT_WIDTH 80

int cli_color_enabled(int mode)
{
    const char *no_color, *term;

    if (mode == CLI_COLOR_ALWAYS)
        return 1;
    if (mode == CLI_COLOR_NEVER)
        return 0;
    if (!isatty(STDOUT_FILENO))
        return 0;
    no_color = getenv("NO_COLOR");
    if (no_color && *no_color)
        return 0;
    term = getenv("TERM");
    return !(term && strcmp(term, "dumb") == 0);
}

/* A positive decimal integer, or 0. */
static long parse_positive(const char *s)
{
    long v = 0;

    if (!s || !*s)
        return 0;
    for (; *s; s++) {
        if (*s < '0' || *s > '9')
            return 0;
        if (v > 100000)
            v = 100000; /* clamped below anyway; avoids overflow */
        else
            v = v * 10 + (*s - '0');
    }
    return v;
}

size_t cli_term_width(void)
{
    long w = parse_positive(getenv("COLUMNS"));

    if (w <= 0 && isatty(STDOUT_FILENO)) {
#ifdef TIOCGWINSZ
        struct winsize ws;

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
            w = ws.ws_col;
#endif
    }
    if (w <= 0)
        w = DEFAULT_WIDTH;
    if (w < MIN_WIDTH)
        w = MIN_WIDTH;
    if (w > MAX_WIDTH)
        w = MAX_WIDTH;
    return (size_t)w;
}

size_t cli_text_width(const char *s, size_t n)
{
    size_t i = 0, w = 0;

    while (i < n) {
        unsigned char c = (unsigned char)s[i];

        if (c == 0x1b) {
            /* An escape sequence: CSI ("ESC [ ... final") or ESC + one byte. */
            i++;
            if (i < n && s[i] == '[') {
                i++;
                while (i < n && !((unsigned char)s[i] >= 0x40 &&
                                  (unsigned char)s[i] <= 0x7e))
                    i++;
            }
            if (i < n)
                i++;
            continue;
        }
        /* Count lead bytes only, so a UTF-8 character is one column. */
        if ((c & 0xC0) != 0x80 && c >= 0x20 && c != 0x7f)
            w++;
        i++;
    }
    return w;
}

void cli_set_style(lc_str *out, int color, unsigned *cur, unsigned want)
{
    static const struct {
        unsigned style;
        const char *code;
    } codes[] = {
        {CLI_ST_BOLD, "1"},     {CLI_ST_ITALIC, "3"}, {CLI_ST_UNDERLINE, "4"},
        {CLI_ST_CODE, "36"},    {CLI_ST_GREEN, "32"}, {CLI_ST_YELLOW, "33"},
        {CLI_ST_RED, "31"},
    };
    size_t i;
    int first = 1;

    if (!color || *cur == want)
        return;
    if (*cur)
        lc_str_puts(out, "\033[0m");
    if (want) {
        lc_str_puts(out, "\033[");
        for (i = 0; i < sizeof codes / sizeof codes[0]; i++) {
            if (!(want & codes[i].style))
                continue;
            if (!first)
                lc_str_putc(out, ';');
            lc_str_puts(out, codes[i].code);
            first = 0;
        }
        lc_str_putc(out, 'm');
    }
    *cur = want;
}

void cli_put_styled(lc_str *out, int color, unsigned style, const char *s)
{
    unsigned cur = 0;

    cli_set_style(out, color, &cur, style);
    lc_str_puts(out, s);
    cli_set_style(out, color, &cur, 0);
}

void cli_put_spaces(lc_str *out, size_t n)
{
    static const char spaces[] = "                                ";

    while (n > 0) {
        size_t k = n < sizeof spaces - 1 ? n : sizeof spaces - 1;
        lc_str_append(out, spaces, k);
        n -= k;
    }
}

const char *cli_difficulty_label(lc_difficulty d)
{
    switch (d) {
    case LC_EASY:
        return "Easy";
    case LC_MEDIUM:
        return "Medium";
    case LC_HARD:
        return "Hard";
    default:
        return "Unknown";
    }
}

unsigned cli_difficulty_style(lc_difficulty d)
{
    switch (d) {
    case LC_EASY:
        return CLI_ST_GREEN;
    case LC_MEDIUM:
        return CLI_ST_YELLOW;
    case LC_HARD:
        return CLI_ST_RED;
    default:
        return 0;
    }
}

/* ------------------------------------------------------------------ */
/* Word wrapping                                                       */
/* ------------------------------------------------------------------ */

static int is_blank(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void cli_wrap_begin(cli_wrap *w, lc_str *out, int color, size_t width,
                    const char *first, const char *rest)
{
    memset(w, 0, sizeof *w);
    w->out = out;
    w->color = color;
    w->width = width;
    w->first = first;
    w->rest = rest;
    lc_str_init(&w->word);
}

static void start_line(cli_wrap *w)
{
    const char *prefix = w->nlines == 0 ? w->first : w->rest;

    w->line_start = w->out->len;
    lc_str_puts(w->out, prefix);
    w->col = cli_text_width(prefix, strlen(prefix));
    w->in_line = 1;
}

static void end_line(cli_wrap *w)
{
    cli_set_style(w->out, w->color, &w->cur, 0);
    /* A line holding only its prefix gets no trailing blanks. */
    if (w->nwords == 0 && !w->out->failed) {
        while (w->out->len > w->line_start &&
               w->out->data[w->out->len - 1] == ' ')
            w->out->len--;
        if (w->out->data)
            w->out->data[w->out->len] = '\0';
    }
    lc_str_putc(w->out, '\n');
    w->in_line = 0;
    w->col = 0;
    w->nwords = 0;
    w->nlines++;
}

/* Places the pending word, on a new line if it doesn't fit on this one. */
static void flush_word(cli_wrap *w)
{
    size_t ww, i;

    if (w->nsegs == 0)
        return;
    ww = cli_text_width(w->word.data, w->word.len);
    if (w->nwords > 0 && w->col + w->gap + ww > w->width)
        end_line(w);
    if (!w->in_line)
        start_line(w);
    if (w->nwords > 0) {
        cli_set_style(w->out, w->color, &w->cur, w->gap_style);
        cli_put_spaces(w->out, w->gap);
        w->col += w->gap;
    }
    for (i = 0; i < w->nsegs; i++) {
        cli_set_style(w->out, w->color, &w->cur, w->segs[i].style);
        lc_str_append(w->out, w->word.data + w->segs[i].off, w->segs[i].len);
    }
    w->col += ww;
    w->nwords++;
    w->nsegs = 0;
    w->gap = 0;
    lc_str_clear(&w->word);
}

static void add_piece(cli_wrap *w, unsigned style, const char *s, size_t n)
{
    cli_seg *last = w->nsegs ? &w->segs[w->nsegs - 1] : NULL;

    if (!last || last->style != style) {
        if (w->nsegs == w->capsegs) {
            size_t ncap = w->capsegs ? w->capsegs * 2 : 8;
            cli_seg *tmp = realloc(w->segs, ncap * sizeof *tmp);
            if (!tmp) {
                w->nomem = 1;
                return;
            }
            w->segs = tmp;
            w->capsegs = ncap;
        }
        last = &w->segs[w->nsegs++];
        last->style = style;
        last->off = w->word.len;
        last->len = 0;
    }
    if (lc_str_append(&w->word, s, n) != LC_OK) {
        w->nomem = 1;
        return;
    }
    last->len += n;
}

void cli_wrap_text(cli_wrap *w, unsigned style, const char *s, size_t n)
{
    size_t i = 0, j;

    while (i < n && !w->nomem) {
        j = i;
        if (is_blank((unsigned char)s[i])) {
            while (j < n && is_blank((unsigned char)s[j]))
                j++;
            flush_word(w);
            if (w->gap == 0)
                w->gap_style = style;
            w->gap += j - i; /* tabs and newlines count as one space each */
        } else {
            while (j < n && !is_blank((unsigned char)s[j]))
                j++;
            add_piece(w, style, s + i, j - i);
        }
        i = j;
    }
}

void cli_wrap_break(cli_wrap *w)
{
    flush_word(w);
    w->gap = 0;
    if (!w->in_line)
        start_line(w);
    end_line(w);
}

int cli_wrap_end(cli_wrap *w)
{
    int nomem;

    flush_word(w);
    if (w->in_line || w->nlines == 0) {
        if (!w->in_line)
            start_line(w);
        end_line(w);
    }
    nomem = w->nomem;
    lc_str_free(&w->word);
    free(w->segs);
    w->segs = NULL;
    w->nsegs = w->capsegs = 0;
    return nomem ? LC_ERR_NOMEM : LC_OK;
}
