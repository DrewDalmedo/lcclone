#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* cmd_list.c: lc list and lc random. */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "cli.h"

#define GAP 2 /* spaces between table columns */

/* The problems that pass the filters, in ID order, in a malloc'ed array. */
static int select_rows(cli *c, lc_ctx *ctx, const lc_problem ***out,
                       size_t *n)
{
    const lc_problem **rows;
    size_t i, total = lc_problem_count(ctx);
    int rc = cli_progress(c, ctx);

    *out = NULL;
    *n = 0;
    if (rc != CLI_EXIT_OK)
        return rc;
    rows = malloc((total ? total : 1) * sizeof *rows);
    if (!rows)
        return cli_error(c, ctx, "out of memory");
    for (i = 0; i < total; i++) {
        const lc_problem *p = lc_problem_at(ctx, i);

        if (cli_matches(c, ctx, p, &rc))
            rows[(*n)++] = p;
        else if (rc != CLI_EXIT_OK) {
            free(rows);
            *n = 0;
            return rc;
        }
    }
    *out = rows;
    return CLI_EXIT_OK;
}

static void list_json(lc_json *j, lc_ctx *ctx, const lc_problem **rows,
                      size_t n)
{
    size_t i;

    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "problems");
    lc_json_begin_array(j);
    for (i = 0; i < n; i++) {
        lc_json_begin_object(j);
        cli_json_problem(j, ctx, rows[i]);
        lc_json_end_object(j);
    }
    lc_json_end_array(j);
    cli_json_warnings(j, ctx, NULL);
    lc_json_end_object(j);
}

/* Appends a cell padded with spaces to `width` columns. */
static void put_cell(lc_str *out, int color, unsigned style, const char *s,
                     size_t width, int right)
{
    size_t w = cli_text_width(s, strlen(s));
    size_t pad = w < width ? width - w : 0;

    if (right)
        cli_put_spaces(out, pad);
    cli_put_styled(out, color, style, s);
    if (!right)
        cli_put_spaces(out, pad);
}

static void put_tags(lc_str *out, const lc_problem *p)
{
    size_t t;

    for (t = 0; t < p->ntags; t++) {
        if (t)
            lc_str_puts(out, ", ");
        lc_str_puts(out, p->tags[t]);
    }
}

/* "Solved", "Attempted" or "" for a status, and its colour. */
static const char *status_label(lc_status st)
{
    return st == LC_SOLVED ? "Solved" : st == LC_ATTEMPTED ? "Attempted" : "";
}

static unsigned status_style(lc_status st)
{
    return st == LC_SOLVED ? CLI_ST_GREEN : st == LC_ATTEMPTED ? CLI_ST_YELLOW
                                                               : 0;
}

static lc_status status_of(lc_ctx *ctx, const lc_problem *p)
{
    lc_progress_entry e;

    return lc_progress(ctx, p->id, &e) == LC_OK ? e.status : LC_TODO;
}

/*
 * A table with a header row; one line per problem, never wrapped. The
 * Status column is there once any of the problems has a status.
 */
static void list_text(lc_str *out, int color, lc_ctx *ctx,
                      const lc_problem **rows, size_t n)
{
    size_t idw = 2, titlew = 5, diffw = 10, statw = 0, i, w;
    char id[32];

    for (i = 0; i < n; i++) {
        w = (size_t)snprintf(id, sizeof id, "%d", rows[i]->id);
        if (w > idw)
            idw = w;
        w = cli_text_width(rows[i]->title, strlen(rows[i]->title));
        if (w > titlew)
            titlew = w;
        w = strlen(cli_difficulty_label(rows[i]->difficulty));
        if (w > diffw)
            diffw = w;
        w = strlen(status_label(status_of(ctx, rows[i])));
        if (w > statw)
            statw = w < 6 ? 6 : w; /* at least as wide as "Status" */
    }

    put_cell(out, color, CLI_ST_BOLD, "ID", idw, 1);
    cli_put_spaces(out, GAP);
    put_cell(out, color, CLI_ST_BOLD, "Title", titlew, 0);
    cli_put_spaces(out, GAP);
    put_cell(out, color, CLI_ST_BOLD, "Difficulty", diffw, 0);
    cli_put_spaces(out, GAP);
    if (statw) {
        put_cell(out, color, CLI_ST_BOLD, "Status", statw, 0);
        cli_put_spaces(out, GAP);
    }
    cli_put_styled(out, color, CLI_ST_BOLD, "Tags");
    lc_str_putc(out, '\n');

    for (i = 0; i < n; i++) {
        const lc_problem *p = rows[i];
        lc_status st = status_of(ctx, p);
        size_t start = out->len;

        snprintf(id, sizeof id, "%d", p->id);
        put_cell(out, 0, 0, id, idw, 1);
        cli_put_spaces(out, GAP);
        put_cell(out, color, 0, p->title, titlew, 0);
        cli_put_spaces(out, GAP);
        put_cell(out, color, cli_difficulty_style(p->difficulty),
                 cli_difficulty_label(p->difficulty), diffw, 0);
        cli_put_spaces(out, GAP);
        if (statw) {
            put_cell(out, color, status_style(st), status_label(st), statw, 0);
            cli_put_spaces(out, GAP);
        }
        put_tags(out, p);
        /* No trailing blanks when the last columns are empty. */
        while (!out->failed && out->len > start &&
               out->data[out->len - 1] == ' ')
            out->len--;
        lc_str_putc(out, '\n');
    }
}

int cmd_list(cli *c)
{
    lc_ctx *ctx;
    const lc_problem **rows = NULL;
    lc_str out = LC_STR_INIT;
    lc_json j;
    size_t n;
    int rc;

    rc = cli_load(c, &ctx);
    if (rc != CLI_EXIT_OK)
        return rc;
    rc = select_rows(c, ctx, &rows, &n);
    if (rc != CLI_EXIT_OK) {
        lc_close(ctx);
        return rc;
    }
    if (c->json) {
        lc_json_init(&j, &out);
        list_json(&j, ctx, rows, n);
        rc = cli_emit_json(c, ctx, &j, &out);
    } else if (n > 0) {
        list_text(&out, cli_color_enabled(c->color), ctx, rows, n);
        rc = out.failed ? cli_error(c, ctx, "out of memory")
                        : cli_write(out.data, out.len);
    }
    lc_str_free(&out);
    free(rows);
    lc_close(ctx);
    return rc;
}

/* A random number below n (n > 0): from /dev/urandom, else the clock. */
static size_t pick(size_t n)
{
    unsigned long long r = 0;
    int fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);

    if (fd < 0 || read(fd, &r, sizeof r) != (ssize_t)sizeof r) {
        struct timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts);
        r = (unsigned long long)ts.tv_sec * 1000000007ull ^
            (unsigned long long)ts.tv_nsec ^
            (unsigned long long)getpid() << 20;
    }
    if (fd >= 0)
        close(fd);
    return (size_t)(r % n);
}

int cmd_random(cli *c)
{
    lc_ctx *ctx;
    const lc_problem **rows = NULL, *p;
    lc_str out = LC_STR_INIT;
    lc_json j;
    size_t n;
    int rc;

    rc = cli_load(c, &ctx);
    if (rc != CLI_EXIT_OK)
        return rc;
    rc = select_rows(c, ctx, &rows, &n);
    if (rc != CLI_EXIT_OK)
        goto done;
    if (n == 0) {
        rc = cli_error(c, ctx, "no problem passes the filters");
        goto done;
    }
    p = rows[pick(n)];
    if (c->json) {
        lc_json_init(&j, &out);
        lc_json_begin_object(&j);
        lc_json_key(&j, "schema");
        lc_json_int(&j, LC_JSON_SCHEMA);
        lc_json_key(&j, "problem");
        lc_json_begin_object(&j);
        cli_json_problem(&j, ctx, p);
        lc_json_end_object(&j);
        lc_json_key(&j, "candidates");
        lc_json_int(&j, (long long)n);
        cli_json_warnings(&j, ctx, NULL);
        lc_json_end_object(&j);
        rc = cli_emit_json(c, ctx, &j, &out);
    } else {
        lc_str_printf(&out, "%d. %s (%s)\n", p->id, p->title,
                      cli_difficulty_label(p->difficulty));
        rc = out.failed ? cli_error(c, ctx, "out of memory")
                        : cli_write(out.data, out.len);
    }
done:
    lc_str_free(&out);
    free(rows);
    lc_close(ctx);
    return rc;
}
