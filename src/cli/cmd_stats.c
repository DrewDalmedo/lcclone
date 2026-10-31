#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * cmd_stats.c: lc stats. Your progress counted by difficulty and tag, or
 * with --manifest how much of the problem set in MANIFEST.tsv is written.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"

/* ------------------------------------------------------------------ */
/* Your progress                                                       */
/* ------------------------------------------------------------------ */

static void json_row(lc_json *j, const char *key, const lc_stats_row *r)
{
    lc_json_begin_object(j);
    lc_json_key(j, key);
    lc_json_string(j, r->name);
    lc_json_key(j, "total");
    lc_json_int(j, (long long)r->total);
    lc_json_key(j, "solved");
    lc_json_int(j, (long long)r->solved);
    lc_json_key(j, "attempted");
    lc_json_int(j, (long long)r->attempted);
    lc_json_end_object(j);
}

static void stats_json(lc_json *j, lc_ctx *ctx, const lc_stats_summary *s)
{
    size_t i;

    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "total");
    lc_json_int(j, (long long)s->all.total);
    lc_json_key(j, "solved");
    lc_json_int(j, (long long)s->all.solved);
    lc_json_key(j, "attempted");
    lc_json_int(j, (long long)s->all.attempted);
    lc_json_key(j, "difficulties");
    lc_json_begin_array(j);
    for (i = LC_EASY; i <= LC_HARD; i++)
        json_row(j, "difficulty", &s->difficulty[i]);
    lc_json_end_array(j);
    lc_json_key(j, "tags");
    lc_json_begin_array(j);
    for (i = 0; i < s->ntags; i++)
        json_row(j, "tag", &s->tags[i]);
    lc_json_end_array(j);
    cli_json_warnings(j, ctx, NULL);
    lc_json_end_object(j);
}

/* A table of rows: a name column `namew` wide, then Solved, Attempted
 * and Total. */
static void table(lc_str *out, int color, const char *title, size_t namew,
                  const lc_stats_row *rows, size_t n, int label)
{
    size_t i, w;

    w = strlen(title);
    cli_put_styled(out, color, CLI_ST_BOLD, title);
    cli_put_spaces(out, namew > w ? namew - w + 2 : 2);
    cli_put_styled(out, color, CLI_ST_BOLD, "Solved  Attempted  Total");
    lc_str_putc(out, '\n');
    for (i = 0; i < n; i++) {
        const char *name = label ? cli_difficulty_label(
                                       lc_difficulty_from_name(rows[i].name))
                                 : rows[i].name;

        w = cli_text_width(name, strlen(name));
        lc_str_puts(out, name);
        cli_put_spaces(out, namew > w ? namew - w + 2 : 2);
        lc_str_printf(out, "%6lu  %9lu  %5lu\n", (unsigned long)rows[i].solved,
                      (unsigned long)rows[i].attempted,
                      (unsigned long)rows[i].total);
    }
}

static void stats_text(lc_str *out, int color, const lc_stats_summary *s)
{
    size_t i, namew = 10;

    if (s->all.total == 0) {
        lc_str_puts(out, "No problems.\n");
        return;
    }
    lc_str_printf(out, "Solved %lu of %lu problems (%lu%%)",
                  (unsigned long)s->all.solved, (unsigned long)s->all.total,
                  (unsigned long)(s->all.solved * 100 / s->all.total));
    if (s->all.attempted)
        lc_str_printf(out, "; attempted %lu more",
                      (unsigned long)s->all.attempted);
    lc_str_puts(out, ".\n\n");
    table(out, color, "Difficulty", 10, &s->difficulty[LC_EASY], 3, 1);
    if (s->ntags == 0)
        return;
    for (i = 0; i < s->ntags; i++) {
        size_t w = cli_text_width(s->tags[i].name, strlen(s->tags[i].name));
        if (w > namew)
            namew = w;
    }
    lc_str_putc(out, '\n');
    table(out, color, "Tag", namew, s->tags, s->ntags, 0);
}

static int progress_stats(cli *c, lc_ctx *ctx)
{
    lc_stats_summary s;
    lc_str out = LC_STR_INIT;
    lc_json j;
    int rc = cli_progress(c, ctx);

    if (rc != CLI_EXIT_OK)
        return rc;
    if (lc_stats(ctx, &s) != LC_OK)
        return cli_error(c, ctx, "%s", lc_last_error(ctx));
    if (c->json) {
        lc_json_init(&j, &out);
        stats_json(&j, ctx, &s);
        rc = cli_emit_json(c, ctx, &j, &out);
    } else {
        stats_text(&out, cli_color_enabled(c->color), &s);
        rc = out.failed ? cli_error(c, ctx, "out of memory")
                        : cli_write(out.data, out.len);
    }
    lc_str_free(&out);
    lc_stats_free(&s);
    return rc;
}

/* ------------------------------------------------------------------ */
/* --manifest                                                          */
/* ------------------------------------------------------------------ */

#define NSTATUS 3
static const char *const status_names[NSTATUS] = {"todo", "draft",
                                                  "verified"};

typedef struct group_count {
    const char *name;         /* a group, or a skip reason */
    size_t n[NSTATUS];        /* by status */
    size_t total;
} group_count;

typedef struct coverage {
    size_t total, premium, skipped;
    size_t by_status[NSTATUS];  /* rows in scope */
    size_t by_difficulty[4];
    group_count *groups;      /* in scope, in first-seen order */
    size_t ngroups;
    group_count *skips;       /* out of scope, by reason */
    size_t nskips;
    int *no_pack;             /* ids marked draft or verified without a pack */
    size_t nno_pack;
    int *unlisted;            /* packs without a row */
    size_t nunlisted;
    int *todo_pack;           /* ids marked todo that have a pack */
    size_t ntodo_pack;
} coverage;

static void coverage_free(coverage *cv)
{
    free(cv->groups);
    free(cv->skips);
    free(cv->no_pack);
    free(cv->unlisted);
    free(cv->todo_pack);
}

static int status_index(const char *st)
{
    int i;

    for (i = 0; i < NSTATUS; i++)
        if (strcmp(status_names[i], st) == 0)
            return i;
    return 0;
}

/* Counts one row into the entry named `name` of *arr. */
static int add_count(group_count **arr, size_t *n, const char *name, int st)
{
    size_t i;

    for (i = 0; i < *n; i++)
        if (strcmp((*arr)[i].name, name) == 0)
            break;
    if (i == *n) {
        group_count *tmp = realloc(*arr, (*n + 1) * sizeof **arr);
        if (!tmp)
            return 0;
        *arr = tmp;
        memset(&tmp[i], 0, sizeof tmp[i]);
        tmp[i].name = name;
        (*n)++;
    }
    (*arr)[i].n[st]++;
    (*arr)[i].total++;
    return 1;
}

static int add_id(int **arr, size_t *n, int id)
{
    int *tmp = realloc(*arr, (*n + 1) * sizeof **arr);

    if (!tmp)
        return 0;
    *arr = tmp;
    tmp[(*n)++] = id;
    return 1;
}

static int count_coverage(lc_ctx *ctx, const lc_manifest *m, coverage *cv)
{
    size_t i;
    int ok = 1;

    memset(cv, 0, sizeof *cv);
    for (i = 0; i < m->count && ok; i++) {
        const lc_manifest_row *r = &m->rows[i];
        int st = status_index(r->status), has_pack;
        char id[16];

        snprintf(id, sizeof id, "%d", r->id);
        has_pack = lc_problem_find(ctx, id) != NULL;

        cv->total++;
        if (r->premium)
            cv->premium++;
        if ((unsigned)r->difficulty < 4)
            cv->by_difficulty[r->difficulty]++;
        if (r->skip) {
            cv->skipped++;
            ok = add_count(&cv->skips, &cv->nskips, r->skip, st);
            continue;
        }
        cv->by_status[st]++;
        ok = add_count(&cv->groups, &cv->ngroups, r->group, st);
        if (ok && st > 0 && !has_pack)
            ok = add_id(&cv->no_pack, &cv->nno_pack, r->id);
        if (ok && st == 0 && has_pack)
            ok = add_id(&cv->todo_pack, &cv->ntodo_pack, r->id);
    }
    for (i = 0; ok && i < lc_problem_count(ctx); i++) {
        const lc_problem *p = lc_problem_at(ctx, i);
        if (!lc_manifest_find(m, p->id))
            ok = add_id(&cv->unlisted, &cv->nunlisted, p->id);
    }
    return ok;
}

static void json_ids(lc_json *j, const char *key, const int *ids, size_t n)
{
    size_t i;

    lc_json_key(j, key);
    lc_json_begin_array(j);
    for (i = 0; i < n; i++)
        lc_json_int(j, ids[i]);
    lc_json_end_array(j);
}

static void json_counts(lc_json *j, const char *key, const char *namekey,
                        const group_count *g, size_t n)
{
    size_t i;
    int k;

    lc_json_key(j, key);
    lc_json_begin_array(j);
    for (i = 0; i < n; i++) {
        lc_json_begin_object(j);
        lc_json_key(j, namekey);
        lc_json_string(j, g[i].name);
        lc_json_key(j, "total");
        lc_json_int(j, (long long)g[i].total);
        for (k = 0; k < NSTATUS; k++) {
            lc_json_key(j, status_names[k]);
            lc_json_int(j, (long long)g[i].n[k]);
        }
        lc_json_end_object(j);
    }
    lc_json_end_array(j);
}

static void manifest_json(lc_json *j, lc_ctx *ctx, const lc_manifest *m,
                          const coverage *cv)
{
    int k;

    lc_json_begin_object(j);
    lc_json_key(j, "schema");
    lc_json_int(j, LC_JSON_SCHEMA);
    lc_json_key(j, "manifest");
    lc_json_string(j, m->path);
    lc_json_key(j, "total");
    lc_json_int(j, (long long)cv->total);
    lc_json_key(j, "premium");
    lc_json_int(j, (long long)cv->premium);
    lc_json_key(j, "skipped");
    lc_json_int(j, (long long)cv->skipped);
    for (k = 0; k < NSTATUS; k++) {
        lc_json_key(j, status_names[k]);
        lc_json_int(j, (long long)cv->by_status[k]);
    }
    json_counts(j, "groups", "group", cv->groups, cv->ngroups);
    json_counts(j, "skips", "reason", cv->skips, cv->nskips);
    json_ids(j, "no_pack", cv->no_pack, cv->nno_pack);
    json_ids(j, "todo_with_pack", cv->todo_pack, cv->ntodo_pack);
    json_ids(j, "unlisted", cv->unlisted, cv->nunlisted);
    cli_json_warnings(j, ctx, NULL);
    lc_json_end_object(j);
}

static void put_ids(lc_str *out, const char *what, const int *ids, size_t n)
{
    size_t i;

    if (n == 0)
        return;
    lc_str_printf(out, "%s:", what);
    for (i = 0; i < n; i++)
        lc_str_printf(out, " %d", ids[i]);
    lc_str_putc(out, '\n');
}

static void manifest_text(lc_str *out, int color, const lc_manifest *m,
                          const coverage *cv)
{
    size_t i, w, namew = 5;
    int k;

    cli_show_path(out, m->path);
    lc_str_printf(out, ": %lu problems (%lu easy, %lu medium, %lu hard), "
                  "%lu premium\n", (unsigned long)cv->total,
                  (unsigned long)cv->by_difficulty[LC_EASY],
                  (unsigned long)cv->by_difficulty[LC_MEDIUM],
                  (unsigned long)cv->by_difficulty[LC_HARD],
                  (unsigned long)cv->premium);
    lc_str_printf(out, "In scope: %lu verified, %lu draft, %lu todo; "
                  "out of scope: %lu\n",
                  (unsigned long)cv->by_status[2],
                  (unsigned long)cv->by_status[1],
                  (unsigned long)cv->by_status[0],
                  (unsigned long)cv->skipped);
    for (i = 0; i < cv->ngroups; i++)
        if ((w = strlen(cv->groups[i].name)) > namew)
            namew = w;
    for (i = 0; i < cv->nskips; i++)
        if ((w = strlen(cv->skips[i].name) + 5) > namew)
            namew = w;
    if (cv->ngroups) {
        lc_str_putc(out, '\n');
        cli_put_styled(out, color, CLI_ST_BOLD, "Group");
        cli_put_spaces(out, namew - 5 + 2);
        cli_put_styled(out, color, CLI_ST_BOLD,
                       "Total  Todo  Draft  Verified");
        lc_str_putc(out, '\n');
        for (i = 0; i < cv->ngroups; i++) {
            const group_count *g = &cv->groups[i];

            lc_str_puts(out, g->name);
            cli_put_spaces(out, namew - strlen(g->name) + 2);
            lc_str_printf(out, "%5lu  %4lu  %5lu  %8lu\n",
                          (unsigned long)g->total, (unsigned long)g->n[0],
                          (unsigned long)g->n[1], (unsigned long)g->n[2]);
        }
    }
    if (cv->nskips) {
        lc_str_putc(out, '\n');
        cli_put_styled(out, color, CLI_ST_BOLD, "Out of scope");
        lc_str_putc(out, '\n');
        for (i = 0; i < cv->nskips; i++) {
            lc_str_printf(out, "skip:%s", cv->skips[i].name);
            cli_put_spaces(out, namew - strlen(cv->skips[i].name) - 5 + 2);
            lc_str_printf(out, "%5lu\n", (unsigned long)cv->skips[i].total);
        }
    }
    k = cv->nno_pack || cv->ntodo_pack || cv->nunlisted;
    if (k)
        lc_str_putc(out, '\n');
    put_ids(out, "Marked draft or verified, but there is no pack",
            cv->no_pack, cv->nno_pack);
    put_ids(out, "Marked todo, but there is a pack", cv->todo_pack,
            cv->ntodo_pack);
    put_ids(out, "Packs that MANIFEST.tsv doesn't list", cv->unlisted,
            cv->nunlisted);
}

static int manifest_stats(cli *c, lc_ctx *ctx)
{
    lc_manifest *m = NULL;
    lc_str out = LC_STR_INIT;
    coverage cv;
    lc_json j;
    int rc;

    if (lc_load_manifest(ctx, &m) != LC_OK)
        return cli_error(c, ctx, "%s", lc_last_error(ctx));
    if (!m)
        return cli_error(c, ctx, "there is no MANIFEST.tsv in %s",
                         lc_problems_dir(ctx));
    if (!count_coverage(ctx, m, &cv)) {
        rc = cli_error(c, ctx, "out of memory");
    } else if (c->json) {
        lc_json_init(&j, &out);
        manifest_json(&j, ctx, m, &cv);
        rc = cli_emit_json(c, ctx, &j, &out);
    } else {
        manifest_text(&out, cli_color_enabled(c->color), m, &cv);
        rc = out.failed ? cli_error(c, ctx, "out of memory")
                        : cli_write(out.data, out.len);
    }
    coverage_free(&cv);
    lc_str_free(&out);
    lc_manifest_free(m);
    return rc;
}

int cmd_stats(cli *c)
{
    lc_ctx *ctx;
    int rc = cli_load(c, &ctx);

    if (rc != CLI_EXIT_OK)
        return rc;
    rc = c->manifest ? manifest_stats(c, ctx) : progress_stats(c, ctx);
    lc_close(ctx);
    return rc;
}
