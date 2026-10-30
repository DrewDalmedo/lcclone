#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* cmd_list.c: lc list. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

#define GAP 2 /* spaces between table columns */

static int matches(const cli *c, const lc_problem *p)
{
    size_t i;

    if (c->difficulties && !(c->difficulties & (1u << p->difficulty)))
        return 0;
    for (i = 0; i < c->ntags; i++)
        if (!lc_problem_has_tag(p, c->tags[i]))
            return 0;
    return 1;
}

static void list_json(lc_json *j, const lc_ctx *ctx, const lc_problem **rows,
                      size_t n)
{
    size_t i, t;

    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "problems");
    lc_json_begin_array(j);
    for (i = 0; i < n; i++) {
        const lc_problem *p = rows[i];

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
        for (t = 0; t < p->ntags; t++)
            lc_json_string(j, p->tags[t]);
        lc_json_end_array(j);
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

/* A table with a header row; one line per problem, never wrapped. */
static void list_text(lc_str *out, int color, const lc_problem **rows,
                      size_t n)
{
    size_t idw = 2, titlew = 5, diffw = 10, i, w;
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
    }

    put_cell(out, color, CLI_ST_BOLD, "ID", idw, 1);
    cli_put_spaces(out, GAP);
    put_cell(out, color, CLI_ST_BOLD, "Title", titlew, 0);
    cli_put_spaces(out, GAP);
    put_cell(out, color, CLI_ST_BOLD, "Difficulty", diffw, 0);
    cli_put_spaces(out, GAP);
    cli_put_styled(out, color, CLI_ST_BOLD, "Tags");
    lc_str_putc(out, '\n');

    for (i = 0; i < n; i++) {
        const lc_problem *p = rows[i];

        snprintf(id, sizeof id, "%d", p->id);
        put_cell(out, 0, 0, id, idw, 1);
        cli_put_spaces(out, GAP);
        put_cell(out, color, 0, p->title, titlew, 0);
        cli_put_spaces(out, GAP);
        if (p->ntags == 0) {
            /* No trailing blanks when the last column is empty. */
            cli_put_styled(out, color, cli_difficulty_style(p->difficulty),
                           cli_difficulty_label(p->difficulty));
        } else {
            put_cell(out, color, cli_difficulty_style(p->difficulty),
                     cli_difficulty_label(p->difficulty), diffw, 0);
            cli_put_spaces(out, GAP);
            put_tags(out, p);
        }
        lc_str_putc(out, '\n');
    }
}

int cmd_list(cli *c)
{
    lc_ctx *ctx;
    const lc_problem **rows;
    lc_str out = LC_STR_INIT;
    lc_json j;
    size_t i, n = 0, total;
    int rc;

    rc = cli_load(c, &ctx);
    if (rc != CLI_EXIT_OK)
        return rc;
    total = lc_problem_count(ctx);
    rows = malloc((total ? total : 1) * sizeof *rows);
    if (!rows) {
        rc = cli_error(c, ctx, "out of memory");
        lc_close(ctx);
        return rc;
    }
    for (i = 0; i < total; i++) {
        const lc_problem *p = lc_problem_at(ctx, i);
        if (matches(c, p))
            rows[n++] = p;
    }

    if (c->json) {
        lc_json_init(&j, &out);
        list_json(&j, ctx, rows, n);
        rc = cli_emit_json(c, ctx, &j, &out);
    } else if (n > 0) {
        list_text(&out, cli_color_enabled(c->color), rows, n);
        rc = out.failed ? cli_error(c, ctx, "out of memory")
                        : cli_write(out.data, out.len);
    }
    lc_str_free(&out);
    free(rows);
    lc_close(ctx);
    return rc;
}
