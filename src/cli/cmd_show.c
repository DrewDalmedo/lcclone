#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * cmd_show.c: lc show. Renders a problem's statement blocks as terminal
 * text, with the @example cases where the statement has its "@examples"
 * line, or prints everything as JSON.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

#define MAX_DEPTH 16      /* list nesting levels tracked for indentation */
#define LIST_INDENT 2     /* top-level list items */
#define CODE_INDENT 4     /* code block lines */
#define EXAMPLE_INDENT 2  /* "Input:", "Output:", "Explanation:" */
#define RAW_INDENT 4      /* input lines shown as they are */
#define MAX_PREFIX 128    /* longest line prefix (indentation and marker) */

typedef struct render {
    lc_str *out;
    int color;
    size_t width;
    int nomem;
    lc_status status;     /* your progress on the problem */
    int any;              /* a block has been written */
    int prev_item;        /* the last block written was a list item */
    size_t item_col[MAX_DEPTH]; /* text column of the last item per depth */
    int item_kind[MAX_DEPTH];   /* 0 none, 1 bulleted, 2 numbered */
} render;

/*
 * Appends a finished block, with a blank line before it unless it is a list
 * item that continues the list of the one before. Empty blocks are dropped.
 */
static void add_block(render *r, lc_str *blk, int is_item, int new_list)
{
    if (blk->failed)
        r->nomem = 1;
    if (blk->len == 0)
        return;
    if (r->any && !(is_item && r->prev_item && !new_list))
        lc_str_putc(r->out, '\n');
    lc_str_append(r->out, blk->data, blk->len);
    r->any = 1;
    r->prev_item = is_item;
}

static void end_wrap(render *r, cli_wrap *w)
{
    if (cli_wrap_end(w) != LC_OK)
        r->nomem = 1;
}

/* n spaces (at most MAX_PREFIX) in buf, which has MAX_PREFIX + 1 bytes. */
static const char *indent(char *buf, size_t n)
{
    if (n > MAX_PREFIX)
        n = MAX_PREFIX;
    memset(buf, ' ', n);
    buf[n] = '\0';
    return buf;
}

static unsigned span_style(const lc_md_span *sp)
{
    unsigned st = 0;

    if (sp->flags & LC_MD_SPAN_STRONG)
        st |= CLI_ST_BOLD;
    if (sp->flags & LC_MD_SPAN_EM)
        st |= CLI_ST_ITALIC;
    if (sp->flags & LC_MD_SPAN_CODE)
        st |= CLI_ST_CODE;
    if (sp->flags & LC_MD_SPAN_LINK)
        st |= CLI_ST_UNDERLINE;
    return st;
}

static int is_link(const lc_md_span *sp)
{
    return (sp->flags & LC_MD_SPAN_LINK) && sp->url;
}

/* 1 if the link text, split over sp[0..n), is exactly its url. */
static int text_is_url(const lc_md_span *sp, size_t n)
{
    const char *url = sp[0].url;
    size_t i, off = 0, ulen = strlen(url);

    for (i = 0; i < n; i++) {
        if (sp[i].len > ulen - off ||
            memcmp(url + off, sp[i].text, sp[i].len) != 0)
            return 0;
        off += sp[i].len;
    }
    return off == ulen;
}

/* One span; a code span is never broken across lines. */
static void wrap_span(cli_wrap *w, const lc_md_span *sp, unsigned base)
{
    if (sp->flags & LC_MD_SPAN_CODE)
        cli_wrap_word(w, base | span_style(sp), sp->text, sp->len);
    else
        cli_wrap_text(w, base | span_style(sp), sp->text, sp->len);
}

/* Feeds inline spans to a wrapper. A link shows as "text (url)", or just
 * as its text when that is the url. */
static void wrap_spans(cli_wrap *w, const lc_md_span *sp, size_t n,
                       unsigned base)
{
    size_t i = 0, start;

    while (i < n) {
        if (sp[i].flags & LC_MD_SPAN_BREAK) {
            cli_wrap_break(w);
            i++;
            continue;
        }
        if (!is_link(&sp[i])) {
            wrap_span(w, &sp[i], base);
            i++;
            continue;
        }
        /* The link's text may be several spans (emphasis, code). */
        start = i;
        while (i < n && is_link(&sp[i]) &&
               strcmp(sp[i].url, sp[start].url) == 0) {
            wrap_span(w, &sp[i], base);
            i++;
        }
        if (!text_is_url(&sp[start], i - start)) {
            const char *url = sp[start].url;

            cli_wrap_text(w, base, " (", 2);
            cli_wrap_text(w, base, url, strlen(url));
            cli_wrap_text(w, base, ")", 1);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Text: statement blocks                                              */
/* ------------------------------------------------------------------ */

/* Wraps a block's spans with the given line prefixes. */
static void inline_block(render *r, lc_str *blk, const lc_md_block *b,
                         unsigned base, const char *first, const char *rest)
{
    cli_wrap w;

    cli_wrap_begin(&w, blk, r->color, r->width, first, rest);
    wrap_spans(&w, b->spans, b->nspans, base);
    end_wrap(r, &w);
}

/* Returns 1 if the item starts a new list: a bulleted item right after a
 * numbered one at the same depth, or the other way round. */
static int list_item(render *r, lc_str *blk, const lc_md_block *b)
{
    char marker[32], first[MAX_PREFIX + 1], rest[MAX_PREFIX + 1];
    size_t depth = b->depth > 0 ? (size_t)b->depth : 0, col, len, k;
    int kind = b->ordered ? 2 : 1, new_list;

    if (depth >= MAX_DEPTH)
        depth = MAX_DEPTH - 1;
    new_list = r->item_kind[depth] != 0 && r->item_kind[depth] != kind;
    r->item_kind[depth] = kind;
    for (k = depth + 1; k < MAX_DEPTH; k++)
        r->item_kind[k] = 0;
    /* A nested item lines up with its parent's text. */
    if (depth > 0 && r->item_col[depth - 1] > 0)
        col = r->item_col[depth - 1];
    else
        col = LIST_INDENT + 2 * depth;
    if (b->ordered)
        snprintf(marker, sizeof marker, "%d.", b->number);
    else
        strcpy(marker, "-");
    len = strlen(marker);
    if (col + len + 1 > MAX_PREFIX)
        col = MAX_PREFIX - len - 1;
    indent(first, col);
    memcpy(first + col, marker, len);
    first[col + len] = ' ';
    first[col + len + 1] = '\0';
    indent(rest, col + len + 1);
    r->item_col[depth] = col + len + 1;
    if (depth + 1 < MAX_DEPTH)
        r->item_col[depth + 1] = 0;
    inline_block(r, blk, b, 0, first, rest);
    return new_list;
}

static void code_block(render *r, lc_str *blk, const lc_md_block *b)
{
    const char *p, *end, *nl;
    unsigned cur = 0;

    if (!b->text)
        return;
    p = b->text;
    end = b->text + b->len;
    while (p < end) {
        nl = memchr(p, '\n', (size_t)(end - p));
        if (!nl)
            nl = end;
        cli_put_spaces(blk, CODE_INDENT);
        cli_set_style(blk, r->color, &cur, CLI_ST_CODE);
        lc_str_append(blk, p, (size_t)(nl - p));
        cli_set_style(blk, r->color, &cur, 0);
        lc_str_putc(blk, '\n');
        p = nl < end ? nl + 1 : end;
    }
}

static void rule(render *r, lc_str *blk)
{
    size_t i;

    for (i = 0; i < r->width; i++)
        lc_str_putc(blk, '-');
    lc_str_putc(blk, '\n');
}

/* ------------------------------------------------------------------ */
/* Text: examples                                                      */
/* ------------------------------------------------------------------ */

static void put_label(render *r, lc_str *blk, const char *label)
{
    cli_put_spaces(blk, EXAMPLE_INDENT);
    cli_put_styled(blk, r->color, CLI_ST_BOLD, label);
}

static void example(render *r, lc_str *blk, const lc_problem *p,
                    const lc_case *c, size_t number)
{
    char title[48], rest[MAX_PREFIX + 1];
    size_t i;

    snprintf(title, sizeof title, "Example %lu:", (unsigned long)number);
    cli_put_styled(blk, r->color, CLI_ST_BOLD, title);
    lc_str_putc(blk, '\n');

    /* "Input: nums = [1,2], target = 3", or the raw lines when the case
     * doesn't have one input per argument name. */
    put_label(r, blk, "Input:");
    if (c->nargs == p->narg_names) {
        for (i = 0; i < c->nargs; i++)
            lc_str_printf(blk, "%s%s = %s", i ? ", " : " ", p->arg_names[i],
                          c->args[i]);
        lc_str_putc(blk, '\n');
    } else {
        lc_str_putc(blk, '\n');
        for (i = 0; i < c->nargs; i++) {
            cli_put_spaces(blk, RAW_INDENT);
            lc_str_puts(blk, c->args[i]);
            lc_str_putc(blk, '\n');
        }
    }
    put_label(r, blk, "Output:");
    lc_str_printf(blk, " %s\n", c->expected);

    if (c->explanation) {
        static const char label[] = "Explanation:";
        char first[MAX_PREFIX + 1];
        cli_wrap w;

        /* Later lines line up under the text after the label. */
        cli_wrap_begin(&w, blk, r->color, r->width,
                       indent(first, EXAMPLE_INDENT),
                       indent(rest, EXAMPLE_INDENT + sizeof label));
        cli_wrap_text(&w, CLI_ST_BOLD, label, sizeof label - 1);
        cli_wrap_text(&w, 0, " ", 1);
        cli_wrap_text(&w, 0, c->explanation, strlen(c->explanation));
        end_wrap(r, &w);
    }
}

static void examples(render *r, const lc_problem *p, const lc_cases *cases)
{
    lc_str blk = LC_STR_INIT;
    size_t i, n = 0;

    if (!cases)
        return;
    for (i = 0; i < cases->count; i++) {
        if (!cases->items[i].example)
            continue;
        lc_str_clear(&blk);
        example(r, &blk, p, &cases->items[i], ++n);
        add_block(r, &blk, 0, 0);
    }
    lc_str_free(&blk);
}

/* ------------------------------------------------------------------ */
/* Text: the whole problem                                             */
/* ------------------------------------------------------------------ */

static void header(render *r, const lc_problem *p)
{
    char num[32], rest[MAX_PREFIX + 1];
    lc_str tags = LC_STR_INIT;
    cli_wrap w;
    size_t t, n;

    n = (size_t)snprintf(num, sizeof num, "%d. ", p->id);
    cli_wrap_begin(&w, r->out, r->color, r->width, "", indent(rest, n));
    cli_wrap_text(&w, CLI_ST_BOLD, num, n);
    cli_wrap_text(&w, CLI_ST_BOLD, p->title, strlen(p->title));
    end_wrap(r, &w);

    lc_str_puts(r->out, "Difficulty: ");
    cli_put_styled(r->out, r->color, cli_difficulty_style(p->difficulty),
                   cli_difficulty_label(p->difficulty));
    lc_str_putc(r->out, '\n');
    if (r->status != LC_TODO) {
        lc_str_puts(r->out, "Status: ");
        cli_put_styled(r->out, r->color,
                       r->status == LC_SOLVED ? CLI_ST_GREEN : CLI_ST_YELLOW,
                       r->status == LC_SOLVED ? "Solved" : "Attempted");
        lc_str_putc(r->out, '\n');
    }

    if (p->ntags > 0) {
        for (t = 0; t < p->ntags; t++) {
            if (t)
                lc_str_puts(&tags, ", ");
            lc_str_puts(&tags, p->tags[t]);
        }
        if (tags.failed)
            r->nomem = 1;
        cli_wrap_begin(&w, r->out, r->color, r->width, "Tags: ",
                       indent(rest, 6));
        cli_wrap_text(&w, 0, lc_str_cstr(&tags), tags.len);
        end_wrap(r, &w);
        lc_str_free(&tags);
    }
    r->any = 1;
}

static void render_text(render *r, const lc_problem *p, const lc_md_doc *doc,
                        const lc_cases *cases)
{
    lc_str blk = LC_STR_INIT;
    int shown = 0, new_list;
    size_t i;

    header(r, p);
    for (i = 0; i < doc->nblocks; i++) {
        const lc_md_block *b = &doc->blocks[i];

        lc_str_clear(&blk);
        new_list = 0;
        if (b->kind != LC_MD_LIST_ITEM) {
            memset(r->item_kind, 0, sizeof r->item_kind);
            memset(r->item_col, 0, sizeof r->item_col);
        }
        switch (b->kind) {
        case LC_MD_PARAGRAPH:
            inline_block(r, &blk, b, 0, "", "");
            break;
        case LC_MD_HEADING:
            inline_block(r, &blk, b, CLI_ST_BOLD, "", "");
            break;
        case LC_MD_CODE:
            code_block(r, &blk, b);
            break;
        case LC_MD_LIST_ITEM:
            new_list = list_item(r, &blk, b);
            break;
        case LC_MD_QUOTE:
            inline_block(r, &blk, b, 0, "| ", "| ");
            break;
        case LC_MD_RULE:
            rule(r, &blk);
            break;
        case LC_MD_EXAMPLES:
            /* Only the first placeholder shows the examples. */
            if (!shown)
                examples(r, p, cases);
            shown = 1;
            break;
        }
        add_block(r, &blk, b->kind == LC_MD_LIST_ITEM, new_list);
    }
    if (!shown)
        examples(r, p, cases);
    lc_str_free(&blk);
}

/* ------------------------------------------------------------------ */
/* JSON                                                                */
/* ------------------------------------------------------------------ */

static const char *block_type(lc_md_kind k)
{
    switch (k) {
    case LC_MD_PARAGRAPH:
        return "paragraph";
    case LC_MD_HEADING:
        return "heading";
    case LC_MD_CODE:
        return "code";
    case LC_MD_LIST_ITEM:
        return "list_item";
    case LC_MD_QUOTE:
        return "quote";
    case LC_MD_RULE:
        return "rule";
    case LC_MD_EXAMPLES:
        return "examples";
    }
    return "unknown";
}

static void json_spans(lc_json *j, const lc_md_block *b)
{
    size_t i;

    lc_json_key(j, "spans");
    lc_json_begin_array(j);
    for (i = 0; i < b->nspans; i++) {
        const lc_md_span *sp = &b->spans[i];

        lc_json_begin_object(j);
        lc_json_key(j, "text");
        lc_json_string_n(j, sp->text, sp->len);
        if (sp->flags & LC_MD_SPAN_STRONG) {
            lc_json_key(j, "strong");
            lc_json_bool(j, 1);
        }
        if (sp->flags & LC_MD_SPAN_EM) {
            lc_json_key(j, "em");
            lc_json_bool(j, 1);
        }
        if (sp->flags & LC_MD_SPAN_CODE) {
            lc_json_key(j, "code");
            lc_json_bool(j, 1);
        }
        if (is_link(sp)) {
            lc_json_key(j, "link");
            lc_json_string(j, sp->url);
        }
        if (sp->flags & LC_MD_SPAN_BREAK) {
            lc_json_key(j, "break");
            lc_json_bool(j, 1);
        }
        lc_json_end_object(j);
    }
    lc_json_end_array(j);
}

static void json_block(lc_json *j, const lc_md_block *b)
{
    lc_json_begin_object(j);
    lc_json_key(j, "type");
    lc_json_string(j, block_type(b->kind));
    lc_json_key(j, "line");
    lc_json_int(j, b->line);
    switch (b->kind) {
    case LC_MD_HEADING:
        lc_json_key(j, "level");
        lc_json_int(j, b->level);
        json_spans(j, b);
        break;
    case LC_MD_CODE:
        lc_json_key(j, "lang");
        lc_json_string(j, b->lang ? b->lang : "");
        lc_json_key(j, "text");
        lc_json_string_n(j, b->text ? b->text : "", b->text ? b->len : 0);
        break;
    case LC_MD_LIST_ITEM:
        lc_json_key(j, "ordered");
        lc_json_bool(j, b->ordered);
        lc_json_key(j, "number");
        if (b->ordered)
            lc_json_int(j, b->number);
        else
            lc_json_null(j);
        lc_json_key(j, "depth");
        lc_json_int(j, b->depth);
        json_spans(j, b);
        break;
    case LC_MD_PARAGRAPH:
    case LC_MD_QUOTE:
        json_spans(j, b);
        break;
    case LC_MD_RULE:
    case LC_MD_EXAMPLES:
        break;
    }
    lc_json_end_object(j);
}

static void json_examples(lc_json *j, const lc_problem *p,
                          const lc_cases *cases)
{
    size_t i, k;

    lc_json_key(j, "examples");
    lc_json_begin_array(j);
    for (i = 0; cases && i < cases->count; i++) {
        const lc_case *c = &cases->items[i];
        int named = c->nargs == p->narg_names;

        if (!c->example)
            continue;
        lc_json_begin_object(j);
        lc_json_key(j, "inputs");
        lc_json_begin_array(j);
        for (k = 0; k < c->nargs; k++) {
            lc_json_begin_object(j);
            lc_json_key(j, "name");
            lc_json_string(j, named ? p->arg_names[k] : NULL);
            lc_json_key(j, "value");
            lc_json_string(j, c->args[k]);
            lc_json_end_object(j);
        }
        lc_json_end_array(j);
        lc_json_key(j, "output");
        lc_json_string(j, c->expected);
        lc_json_key(j, "explanation");
        lc_json_string(j, c->explanation);
        lc_json_end_object(j);
    }
    lc_json_end_array(j);
}

/* The function lc's harness calls, in each language; null for a problem
 * without a signature. */
static int json_prototypes(lc_json *j, lc_ctx *ctx, const lc_problem *p)
{
    lc_str s = LC_STR_INIT;
    size_t i;
    int rc = LC_OK;

    lc_json_key(j, "prototypes");
    if (!p->sig) {
        lc_json_null(j);
        return LC_OK;
    }
    lc_json_begin_object(j);
    for (i = 0; i < lc_lang_count() && rc == LC_OK; i++) {
        lc_str_clear(&s);
        rc = lc_prototype(ctx, p, lc_lang_at(i), &s);
        lc_json_key(j, lc_lang_at(i)->name);
        lc_json_string(j, rc == LC_OK ? s.data : NULL);
    }
    lc_json_end_object(j);
    lc_str_free(&s);
    return rc;
}

static int render_json(lc_json *j, lc_ctx *ctx, const lc_problem *p,
                       const lc_md_doc *doc, const lc_cases *cases,
                       const char *cases_warning)
{
    lc_progress_entry e;
    size_t i;
    int rc;

    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "problem");
    lc_json_begin_object(j);
    lc_json_key(j, "id");
    lc_json_int(j, p->id);
    lc_json_key(j, "slug");
    lc_json_string(j, p->slug);
    lc_json_key(j, "title");
    lc_json_string(j, p->title);
    lc_json_key(j, "difficulty");
    lc_json_string(j, lc_difficulty_name(p->difficulty));
    lc_json_key(j, "tags");
    lc_json_begin_array(j);
    for (i = 0; i < p->ntags; i++)
        lc_json_string(j, p->tags[i]);
    lc_json_end_array(j);
    /* On failure (cli_progress already read it) e is an empty entry. */
    (void)lc_progress(ctx, p->id, &e);
    lc_json_key(j, "status");
    lc_json_string(j, lc_status_name(e.status));
    lc_json_key(j, "progress");
    lc_json_begin_object(j);
    lc_json_key(j, "lang");
    lc_json_string(j, *e.lang ? e.lang : NULL);
    lc_json_key(j, "best_ms");
    if (e.best_ms >= 0)
        lc_json_int(j, e.best_ms);
    else
        lc_json_null(j);
    lc_json_key(j, "last_submitted");
    if (e.last_ts > 0)
        lc_json_int(j, e.last_ts);
    else
        lc_json_null(j);
    lc_json_end_object(j);
    lc_json_key(j, "signature");
    lc_json_string(j, p->signature);
    rc = json_prototypes(j, ctx, p);
    lc_json_key(j, "time_limit_ms");
    lc_json_int(j, p->time_limit_ms);
    lc_json_key(j, "memory_limit_mb");
    lc_json_int(j, p->memory_limit_mb);
    lc_json_key(j, "dir");
    lc_json_string(j, p->dir);
    lc_json_key(j, "statement");
    lc_json_string_n(j, p->statement ? p->statement : "",
                     p->statement ? p->statement_len : 0);
    lc_json_key(j, "statement_line");
    lc_json_int(j, p->statement_line);
    lc_json_key(j, "blocks");
    lc_json_begin_array(j);
    for (i = 0; i < doc->nblocks; i++)
        json_block(j, &doc->blocks[i]);
    lc_json_end_array(j);
    json_examples(j, p, cases);
    lc_json_end_object(j);
    cli_json_warnings(j, ctx, cases_warning);
    lc_json_end_object(j);
    return rc;
}

/* ------------------------------------------------------------------ */
/* The command                                                         */
/* ------------------------------------------------------------------ */

int cmd_show(cli *c)
{
    lc_ctx *ctx;
    const lc_problem *p;
    lc_cases *cases = NULL;
    lc_md_doc *doc = NULL;
    lc_str out = LC_STR_INIT, warning = LC_STR_INIT;
    lc_json j;
    render r;
    int rc;

    rc = cli_load(c, &ctx);
    if (rc != CLI_EXIT_OK)
        return rc;
    p = lc_problem_find(ctx, c->args[0]);
    if (!p) {
        rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
        goto done;
    }
    /* A broken tests.txt costs the examples, not the statement. */
    rc = lc_load_cases(ctx, p, &cases);
    if (rc == LC_ERR_NOMEM) {
        rc = cli_error(c, ctx, "out of memory");
        goto done;
    }
    if (rc != LC_OK) {
        if (lc_str_puts(&warning, lc_last_error(ctx)) != LC_OK) {
            rc = cli_error(c, ctx, "out of memory");
            goto done;
        }
        cli_warn(warning.data);
        cases = NULL;
    }
    if (lc_md_parse(p->statement, p->statement_len, &doc) != LC_OK) {
        rc = cli_error(c, ctx, "out of memory");
        goto done;
    }
    rc = cli_progress(c, ctx);
    if (rc != CLI_EXIT_OK)
        goto done;

    if (c->json) {
        lc_json_init(&j, &out);
        if (render_json(&j, ctx, p, doc, cases, warning.data) != LC_OK)
            rc = cli_error(c, ctx, "%s", lc_last_error(ctx));
        else
            rc = cli_emit_json(c, ctx, &j, &out);
        goto done;
    }
    memset(&r, 0, sizeof r);
    r.out = &out;
    r.color = cli_color_enabled(c->color);
    r.width = cli_term_width();
    {
        lc_progress_entry e;

        r.status = lc_progress(ctx, p->id, &e) == LC_OK ? e.status : LC_TODO;
    }
    render_text(&r, p, doc, cases);
    if (r.nomem || out.failed)
        rc = cli_error(c, ctx, "out of memory");
    else
        rc = cli_page(out.data, out.len, !c->no_pager);
done:
    lc_md_free(doc);
    lc_cases_free(cases);
    lc_str_free(&warning);
    lc_str_free(&out);
    lc_close(ctx);
    return rc;
}
