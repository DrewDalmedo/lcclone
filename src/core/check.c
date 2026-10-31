#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * check.c: lc check, which checks a problem pack for its author
 * (docs/authoring.md, "lc check"), and lc check --regen, which rewrites
 * its tests.txt.
 *
 * Everything wrong with a pack is a finding, delivered as an event as
 * soon as it is found and kept in the report. The steps:
 *
 *   1. the pack agrees with its row in MANIFEST.tsv;
 *   2. (--regen) tests.txt is rewritten: @gen cases from gen.c, expected
 *      results from ref;
 *   3. tests.txt reads, and every case fits the signature;
 *   4. the C and C++ stubs build with lc's harness;
 *   5. ref builds and passes every case, well within the time limit;
 *   6. the @gen cases are what gen.c makes now;
 *   7. ref and brute agree on random small cases from gen.c.
 *
 * A problem lc can't judge yet stops after step 3.
 */

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "internal.h"

/* Failures of ref listed one by one; past this, only counted. */
#define MAX_CASE_FINDINGS 10
/* Largest output kept from gen. */
#define GEN_OUTPUT_MAX (256 * LC_MIB)
/* Limits for running gen. */
#define GEN_CPU_SEC 20
#define GEN_WALL_MS 60000
#define GEN_MEMORY ((unsigned long long)2048 << 20)
/* ref may use at most this share of the time limit. */
#define TIME_SHARE 3
/* Longest value shown whole in a finding's detail. */
#define VALUE_MAX 200

const char *lc_finding_level_name(lc_finding_level level)
{
    switch (level) {
    case LC_FINDING_WARNING:
        return "warning";
    case LC_FINDING_NOTE:
        return "note";
    default:
        return "error";
    }
}

/* A report and what it owns. */
typedef struct check_box {
    lc_check_report pub;      /* first: a pointer to it is one to the box */
    lc_arena arena;
    lc_finding *findings;     /* malloc'ed */
    size_t cap;
} check_box;

/* The state of one lc_check. */
typedef struct checker {
    lc_ctx *ctx;
    const lc_problem *p;
    const lc_hooks *hooks;    /* the caller's: build and finding events */
    lc_hooks quiet;           /* for runs: the cancel flag only */
    lc_check_report *r;
    check_box *box;
    lc_arena tmp;             /* builds and paths, freed at the end */
    char *problem_md;         /* <dir>/problem.md */
    char *tests;              /* <dir>/tests.txt */
    char *ref, *brute, *gen;  /* the pack's sources, or NULL */
    const lc_lang *ref_lang, *brute_lang;
    const char *ref_exe, *brute_exe, *gen_exe; /* built programs */
    int ref_tried, brute_tried, gen_tried;     /* a build was attempted */
    int gen_failed;           /* running gen failed (and was reported) */
    int regen_bailed;         /* --regen stopped at something a later step
                                 reports */
} checker;

void lc_check_report_free(lc_check_report *report)
{
    check_box *box = (check_box *)report;

    if (!box)
        return;
    free(box->findings);
    lc_arena_free(&box->arena);
    free(box);
}

static int cancelled(const checker *ck)
{
    return ck->hooks && ck->hooks->cancel && *ck->hooks->cancel;
}

/* ------------------------------------------------------------------ */
/* Findings                                                            */
/* ------------------------------------------------------------------ */

static int vfinding(checker *ck, lc_finding_level level, const char *file,
                    int line, const char *detail, const char *fmt,
                    va_list ap)
{
    lc_arena *a = &ck->box->arena;
    lc_finding *f, *tmp;
    lc_event ev;

    tmp = lc_grow(ck->box->findings, &ck->box->cap, ck->r->nfindings + 1,
                  sizeof *tmp);
    if (!tmp)
        return lc_oom(ck->ctx);
    ck->box->findings = tmp;
    ck->r->findings = tmp;
    f = &tmp[ck->r->nfindings];
    memset(f, 0, sizeof *f);
    f->level = level;
    f->line = line;
    f->file = file ? lc_arena_strdup(a, file) : NULL;
    f->message = lc_arena_vprintf(a, fmt, ap);
    f->detail = detail && *detail ? lc_arena_strdup(a, detail) : NULL;
    if ((file && !f->file) || !f->message || (detail && *detail && !f->detail))
        return lc_oom(ck->ctx);
    ck->r->nfindings++;
    if (level == LC_FINDING_ERROR)
        ck->r->errors++;
    else if (level == LC_FINDING_WARNING)
        ck->r->warnings++;
    if (ck->hooks && ck->hooks->on_event) {
        memset(&ev, 0, sizeof ev);
        ev.kind = LC_EV_FINDING;
        ev.finding = f;
        ck->hooks->on_event(ck->hooks->ud, &ev);
    }
    return LC_OK;
}

static int finding(checker *ck, lc_finding_level level, const char *file,
                   int line, const char *detail, const char *fmt, ...)
    LC_PRINTF(6, 7);

static int finding(checker *ck, lc_finding_level level, const char *file,
                   int line, const char *detail, const char *fmt, ...)
{
    va_list ap;
    int rc;

    va_start(ap, fmt);
    rc = vfinding(ck, level, file, line, detail, fmt, ap);
    va_end(ap);
    return rc;
}

/*
 * A finding from the context's last error. A message that starts with
 * "<file>:<line>: " for one of the pack's files is split into the two.
 */
static int error_finding(checker *ck, lc_finding_level level,
                         const char *detail)
{
    const char *files[3], *msg;
    lc_str copy = LC_STR_INIT;
    size_t i;
    int rc;

    if (ck->ctx->err_nomem)
        return LC_ERR_NOMEM;
    if (lc_str_puts(&copy, lc_last_error(ck->ctx)) != LC_OK)
        return lc_oom(ck->ctx);
    msg = copy.data;
    files[0] = ck->tests;
    files[1] = ck->problem_md;
    files[2] = NULL;
    for (i = 0; files[i]; i++) {
        size_t n = strlen(files[i]);
        const char *q = msg + n;
        long line = 0;

        if (strncmp(msg, files[i], n) != 0 || *q != ':')
            continue;
        for (q++; *q >= '0' && *q <= '9' && line < 100000000; q++)
            line = line * 10 + (*q - '0');
        if (q > msg + n + 1 && q[0] == ':' && q[1] == ' ') {
            rc = finding(ck, level, files[i], (int)line, detail, "%s", q + 2);
            lc_str_free(&copy);
            return rc;
        }
    }
    rc = finding(ck, level, NULL, 0, detail, "%s", msg);
    lc_str_free(&copy);
    return rc;
}

/* A value, cut when it is long (a case at the limits can be megabytes). */
static void put_value(lc_str *s, const char *v)
{
    size_t n, cut = VALUE_MAX;

    if (!v) {
        lc_str_puts(s, "(none)");
        return;
    }
    n = strlen(v);
    if (n <= VALUE_MAX) {
        lc_str_append(s, v, n);
        return;
    }
    while (cut > 0 && ((unsigned char)v[cut] & 0xC0) == 0x80)
        cut--;
    lc_str_append(s, v, cut);
    lc_str_printf(s, "... (%lu bytes in all)", (unsigned long)n);
}

/* The detail of a case: its named inputs, then labelled values. */
static void case_detail(lc_str *s, const lc_problem *p,
                        const char *const *inputs, size_t n,
                        const char *label1, const char *v1,
                        const char *label2, const char *v2)
{
    size_t i;

    lc_str_puts(s, "Input:   ");
    if (n == p->narg_names) {
        for (i = 0; i < n; i++) {
            lc_str_printf(s, "%s%s = ", i ? ", " : " ", p->arg_names[i]);
            put_value(s, inputs[i]);
        }
        lc_str_putc(s, '\n');
    } else {
        lc_str_putc(s, '\n');
        for (i = 0; i < n; i++) {
            lc_str_puts(s, "    ");
            put_value(s, inputs[i]);
            lc_str_putc(s, '\n');
        }
    }
    if (label1) {
        lc_str_printf(s, "%-9s ", label1);
        put_value(s, v1);
        lc_str_putc(s, '\n');
    }
    if (label2) {
        lc_str_printf(s, "%-9s ", label2);
        put_value(s, v2);
        lc_str_putc(s, '\n');
    }
}

/* The part of a path after the pack's directory, for messages. */
static const char *short_name(const checker *ck, const char *path)
{
    size_t n = strlen(ck->p->dir);

    if (strncmp(path, ck->p->dir, n) == 0 && path[n] == '/')
        return path + n + 1;
    return path;
}

/* ------------------------------------------------------------------ */
/* Building                                                            */
/* ------------------------------------------------------------------ */

/*
 * Builds one of the pack's sources. Returns LC_OK with *exe set when it
 * built; LC_OK with *exe NULL after a finding (or when cancelled); or an
 * error that stops the check.
 */
static int build(checker *ck, const lc_lang *lang, const char *src,
                 int role, const char *what, const char **exe)
{
    lc_build *b = NULL;
    int rc, stopped = 0;

    *exe = NULL;
    rc = lc_build_step(ck->ctx, ck->p, lang, src, lang->default_std, 0, role,
                       ck->hooks, &ck->tmp, &b, exe, &stopped);
    if (rc == LC_ERR_NOMEM || ck->ctx->err_nomem)
        return LC_ERR_NOMEM;
    if (rc != LC_OK)
        return finding(ck, LC_FINDING_ERROR, src, 0, NULL, "%s can't be "
                       "built: %s", what, lc_last_error(ck->ctx));
    if (stopped) {
        ck->r->cancelled = 1;
        return LC_OK;
    }
    if (!b->ok)
        return finding(ck, LC_FINDING_ERROR, src, 0, b->output, "%s does not "
                       "compile with lc's %s (%s, %s)", what,
                       role == LC_ROLE_GEN ? "lc_gen.h" : "harness",
                       lang->title, b->std);
    return LC_OK;
}

static int ensure_ref(checker *ck)
{
    if (!ck->ref_tried && ck->ref) {
        ck->ref_tried = 1;
        return build(ck, ck->ref_lang, ck->ref, LC_ROLE_REF,
                     short_name(ck, ck->ref), &ck->ref_exe);
    }
    return LC_OK;
}

static int ensure_gen(checker *ck)
{
    if (!ck->gen_tried && ck->gen) {
        ck->gen_tried = 1;
        return build(ck, lc_lang_find("c"), ck->gen, LC_ROLE_GEN, "gen.c",
                     &ck->gen_exe);
    }
    return LC_OK;
}

static int ensure_brute(checker *ck)
{
    if (!ck->brute_tried && ck->brute) {
        ck->brute_tried = 1;
        return build(ck, ck->brute_lang, ck->brute, LC_ROLE_BRUTE,
                     short_name(ck, ck->brute), &ck->brute_exe);
    }
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Running gen                                                         */
/* ------------------------------------------------------------------ */

/*
 * Runs gen for `count` cases from `seed` and parses what it prints. On
 * LC_OK *cases is the list (its count may differ from `count`), or NULL
 * after a finding or when cancelled. `about` names the cases in messages,
 * at file:line `at`.
 */
static int run_gen(checker *ck, unsigned long long seed, int small,
                   unsigned long long count, const char *at_file, int at_line,
                   const char *about, lc_cases **cases)
{
    lc_run_spec spec;
    lc_run_result r;
    lc_str why = LC_STR_INIT, name = LC_STR_INIT;
    char seedbuf[24], countbuf[24];
    char *argv[5];
    int rc;

    *cases = NULL;
    snprintf(seedbuf, sizeof seedbuf, "%llu", seed);
    snprintf(countbuf, sizeof countbuf, "%llu", count);
    argv[0] = (char *)ck->gen_exe;
    argv[1] = seedbuf;
    argv[2] = small ? "small" : "large";
    argv[3] = countbuf;
    argv[4] = NULL;
    memset(&spec, 0, sizeof spec);
    spec.path = ck->gen_exe;
    spec.argv = argv;
    spec.cwd = lc_dir_of(&ck->tmp, ck->gen_exe);
    spec.cpu_sec = GEN_CPU_SEC;
    spec.wall_ms = GEN_WALL_MS;
    spec.as_bytes = GEN_MEMORY;
    spec.stdout_max = GEN_OUTPUT_MAX;
    spec.cancel = ck->quiet.cancel;
    if (!spec.cwd)
        return lc_oom(ck->ctx);
    lc_run_result_init(&r);
    rc = lc_run(ck->ctx, &spec, &r);
    if (rc != LC_OK) {
        rc = ck->ctx->err_nomem ? LC_ERR_NOMEM
                                : finding(ck, LC_FINDING_ERROR, ck->gen, 0,
                                          NULL, "gen.c can't be run: %s",
                                          lc_last_error(ck->ctx));
        goto done;
    }
    if (r.cancelled) {
        ck->r->cancelled = 1;
        goto done;
    }
    if (!r.exited || r.exit_code != 0 || r.overflow) {
        ck->gen_failed = 1;
        if (r.overflow & LC_RUN_STDOUT)
            lc_str_puts(&why, "printed more than 256 MiB");
        else if (r.overflow)
            lc_str_puts(&why, "wrote more than 1 MiB on stderr");
        else if (r.timed_out)
            lc_str_printf(&why, "still running after %d s", GEN_WALL_MS / 1000);
        else if (r.signal)
            lc_str_printf(&why, "killed by %s", lc_signal_name(r.signal)
                                                    ? lc_signal_name(r.signal)
                                                    : "a signal");
        else
            lc_str_printf(&why, "exit status %d", r.exit_code);
        rc = why.failed ? lc_oom(ck->ctx)
                        : finding(ck, LC_FINDING_ERROR, at_file, at_line,
                                  lc_str_cstr(&r.err), "gen.c failed making "
                                  "%s (%s)", about, why.data);
        goto done;
    }
    lc_str_printf(&name, "the output of gen.c for %s", about);
    if (name.failed) {
        rc = lc_oom(ck->ctx);
        goto done;
    }
    rc = lc_parse_user_cases(ck->ctx, lc_str_cstr(&r.out), r.out.len,
                             name.data, cases);
    if (rc == LC_ERR_NOMEM)
        goto done;
    if (rc != LC_OK) {
        ck->gen_failed = 1;
        rc = finding(ck, LC_FINDING_ERROR, at_file, at_line, NULL, "%s",
                     lc_last_error(ck->ctx));
    }
done:
    lc_run_result_free(&r);
    lc_str_free(&why);
    lc_str_free(&name);
    return rc;
}

/* The seed and size of an @gen case ("7" or "7 small"). */
static unsigned long long gen_args(const char *gen, int *small)
{
    unsigned long long v = 0;

    for (; *gen >= '0' && *gen <= '9'; gen++)
        v = v * 10 + (unsigned)(*gen - '0');
    *small = strcmp(gen, " small") == 0;
    return v;
}

/*
 * Makes the inputs of the case marked "@gen <args>" at tests.txt:line.
 * On LC_OK *made is the case, or NULL after a finding.
 */
static int gen_case(checker *ck, const char *args, int line, lc_cases **made)
{
    lc_str about = LC_STR_INIT;
    unsigned long long seed;
    int small, rc;

    *made = NULL;
    seed = gen_args(args, &small);
    if (lc_str_printf(&about, "'@gen %s'", args) != LC_OK)
        return lc_oom(ck->ctx);
    rc = run_gen(ck, seed, small, 1, ck->tests, line, about.data, made);
    if (rc == LC_OK && *made && (*made)->count != 1) {
        ck->gen_failed = 1;
        rc = finding(ck, LC_FINDING_ERROR, ck->tests, line, NULL, "gen.c "
                     "printed %lu cases for %s instead of one",
                     (unsigned long)(*made)->count, about.data);
        lc_cases_free(*made);
        *made = NULL;
    }
    lc_str_free(&about);
    return rc;
}

/* 1 if two cases have the same input lines. */
static int same_inputs(const lc_case *a, const lc_case *b)
{
    size_t i;

    if (a->nargs != b->nargs)
        return 0;
    for (i = 0; i < a->nargs; i++)
        if (strcmp(a->args[i], b->args[i]) != 0)
            return 0;
    return 1;
}

/* ------------------------------------------------------------------ */
/* 1. The manifest                                                     */
/* ------------------------------------------------------------------ */

/* The line of a header field in problem.md, or 0. */
static int field_line(const lc_problem *p, const char *key)
{
    size_t i;

    for (i = 0; i < p->nfields; i++)
        if (strcmp(p->fields[i].key, key) == 0)
            return p->fields[i].line;
    return 0;
}

static int check_manifest(checker *ck, const lc_manifest *m)
{
    const lc_problem *p = ck->p;
    const lc_manifest_row *row;
    const char *name;
    int rc = LC_OK;

    if (!m)
        return LC_OK;
    name = strrchr(m->path, '/');
    name = name ? name + 1 : m->path;
    row = lc_manifest_find(m, p->id);
    if (!row)
        return finding(ck, LC_FINDING_ERROR, ck->problem_md,
                       field_line(p, "id"), NULL, "problem %d is not in %s",
                       p->id, name);
    if (strcmp(row->slug, p->slug) != 0)
        rc = finding(ck, LC_FINDING_ERROR, ck->problem_md,
                     field_line(p, "slug"), NULL, "slug '%s' differs from "
                     "'%s' in %s:%d", p->slug, row->slug, name, row->line);
    if (rc == LC_OK && strcmp(row->title, p->title) != 0)
        rc = finding(ck, LC_FINDING_ERROR, ck->problem_md,
                     field_line(p, "title"), NULL, "title '%s' differs from "
                     "'%s' in %s:%d", p->title, row->title, name, row->line);
    if (rc == LC_OK && row->difficulty != p->difficulty)
        rc = finding(ck, LC_FINDING_ERROR, ck->problem_md,
                     field_line(p, "difficulty"), NULL, "difficulty '%s' "
                     "differs from '%s' in %s:%d",
                     lc_difficulty_name(p->difficulty),
                     lc_difficulty_name(row->difficulty), name, row->line);
    if (rc == LC_OK && row->skip)
        rc = finding(ck, LC_FINDING_WARNING, m->path, row->line, NULL,
                     "problem %d is out of scope (skip:%s), but has a pack",
                     p->id, row->skip);
    else if (rc == LC_OK && strcmp(row->status, "todo") == 0)
        rc = finding(ck, LC_FINDING_WARNING, m->path, row->line, NULL,
                     "problem %d has a pack, but is still marked todo",
                     p->id);
    return rc;
}

/* ------------------------------------------------------------------ */
/* 2. --regen                                                          */
/* ------------------------------------------------------------------ */

/* One line of a text: where it starts, its length without the line end
 * ("\n" or "\r\n"), and its length with it. */
typedef struct text_line {
    const char *p;
    size_t n;
    size_t raw;
} text_line;

/* What --regen puts in a case. */
typedef struct regen_case {
    const lc_case *new_inputs; /* gen's case, or NULL to keep them */
    const char *expected;      /* the new "=>" value, or NULL to keep it */
} regen_case;

/* 1 if a line is a comment ('#' after any blanks). */
static int is_comment(const text_line *l)
{
    size_t i = 0;

    while (i < l->n && (l->p[i] == ' ' || l->p[i] == '\t'))
        i++;
    return i < l->n && l->p[i] == '#';
}

/*
 * Writes `text` with the regenerated cases into `out`. Only the lines of
 * those cases change: every other line is copied as it was, line end
 * included. New lines end like the file's first line.
 */
static int compose(checker *ck, const char *text, size_t len,
                   const lc_cases *cases, const regen_case *rg, lc_str *out)
{
    text_line *lines = NULL;
    const char **replace = NULL;
    lc_str *after = NULL;
    const char *eol = "\n", *p = text, *end = text + len;
    size_t nlines = 0, cap = 0, i, k;
    int rc = LC_OK;

    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        text_line *tmp = lc_grow(lines, &cap, nlines + 1, sizeof *lines);
        text_line *l;

        if (!tmp) {
            rc = lc_oom(ck->ctx);
            goto done;
        }
        lines = tmp;
        l = &lines[nlines++];
        l->p = p;
        l->raw = nl ? (size_t)(nl - p) + 1 : (size_t)(end - p);
        l->n = nl ? (size_t)(nl - p) : l->raw;
        if (nl && l->n && p[l->n - 1] == '\r')
            l->n--;
        p = nl ? nl + 1 : end;
    }
    if (nlines && lines[0].raw > lines[0].n + 1)
        eol = "\r\n";

    /* Line numbers count from 1; slot 0 is unused. */
    replace = calloc(nlines + 1, sizeof *replace);
    after = calloc(nlines + 1, sizeof *after);
    if (!replace || !after) {
        rc = lc_oom(ck->ctx);
        goto done;
    }
    for (i = 0; i < cases->count; i++) {
        const lc_case *c = &cases->items[i];
        int ins = c->last_line;   /* where an "=>" line would go */

        if (rg[i].new_inputs) {
            const lc_case *g = rg[i].new_inputs;
            lc_str block = LC_STR_INIT;
            char *copy;

            for (k = 0; k < g->nargs; k++)
                lc_str_printf(&block, "%s%s", g->args[k], eol);
            if (g->nargs == 0)
                lc_str_printf(&block, "()%s", eol);
            copy = block.failed ? NULL
                                : lc_arena_strdup(&ck->tmp, block.data);
            lc_str_free(&block);
            if (!copy) {
                rc = lc_oom(ck->ctx);
                goto done;
            }
            if (c->last_line > 0) {
                /* The old inputs go, but not the comments among them; the
                 * new inputs take the first one's place. */
                for (k = (size_t)c->line; k <= (size_t)c->last_line; k++)
                    if (!is_comment(&lines[k - 1]))
                        replace[k] = "";
                replace[c->line] = copy;
            } else {
                lc_str_puts(&after[c->line], copy);
                ins = c->line;
            }
        }
        if (!rg[i].expected)
            continue;
        if (c->expected_line > 0) {
            const text_line *l = &lines[c->expected_line - 1];
            size_t ind = 0;

            while (ind < l->n && (l->p[ind] == ' ' || l->p[ind] == '\t'))
                ind++;
            replace[c->expected_line] = lc_arena_printf(
                &ck->tmp, "%.*s=> %s%.*s", (int)ind, l->p, rg[i].expected,
                (int)(l->raw - l->n), l->p + l->n);
            if (!replace[c->expected_line]) {
                rc = lc_oom(ck->ctx);
                goto done;
            }
            continue;
        }
        lc_str_printf(&after[ins > 0 ? ins : c->line], "=> %s%s",
                      rg[i].expected, eol);
    }

    for (i = 1; i <= nlines; i++) {
        const text_line *l = &lines[i - 1];

        if (replace[i])
            lc_str_puts(out, replace[i]);
        else
            lc_str_append(out, l->p, l->raw);
        if (after[i].len) {
            /* The last line may have no line end yet. */
            if (out->len && lc_str_cstr(out)[out->len - 1] != '\n')
                lc_str_puts(out, eol);
            lc_str_append(out, after[i].data, after[i].len);
        }
        if (after[i].failed)
            out->failed = 1;
    }
    if (out->failed)
        rc = lc_oom(ck->ctx);
done:
    if (after)
        for (i = 0; i <= nlines; i++)
            lc_str_free(&after[i]);
    free(after);
    free(replace);
    free(lines);
    return rc;
}

/* 1 if the literal `old` (may be NULL) is a result matching `now` under
 * the problem's comparison, so that --regen keeps it. */
static int still_matches(checker *ck, const char *old, const char *now)
{
    lc_pool *pool;
    const lc_value *a, *b;
    int match = 0;

    if (!old)
        return 0;
    pool = lc_pool_new();
    if (!pool)
        return -1;
    if (lc_value_parse(ck->ctx, pool, lc_result_type(ck->p), old,
                       strlen(old), &a) == LC_OK &&
        lc_value_parse(ck->ctx, pool, lc_result_type(ck->p), now,
                       strlen(now), &b) == LC_OK &&
        lc_value_match(ck->ctx, &ck->p->compare, b, a, &match) != LC_OK)
        match = 0;
    lc_pool_free(pool);
    return ck->ctx->err_nomem ? -1 : match;
}

/* --regen stops at a problem that a later step reports: says only that
 * tests.txt was left as it was. */
static int bail(checker *ck)
{
    ck->regen_bailed = 1;
    return finding(ck, LC_FINDING_NOTE, ck->tests, 0, NULL, "--regen: "
                   "tests.txt was not rewritten, because of the errors "
                   "below");
}

static int regen(checker *ck)
{
    lc_str text = LC_STR_INIT, out = LC_STR_INIT, why = LC_STR_INIT;
    lc_cases *cases = NULL, **made = NULL;
    regen_case *rg = NULL;
    const char *cwd;
    size_t i, generated = 0, changed = 0;
    int rc, any_gen = 0;

    rc = lc_read_file(ck->ctx, ck->tests, LC_MAX_TESTS_TXT, &text);
    if (rc == LC_ERR_NOTFOUND) {
        rc = finding(ck, LC_FINDING_ERROR, ck->tests, 0, NULL, "--regen: "
                     "there is no tests.txt to rewrite");
        goto done;
    }
    if (rc != LC_OK) {
        rc = ck->ctx->err_nomem ? LC_ERR_NOMEM : error_finding(ck,
                                                     LC_FINDING_ERROR, NULL);
        goto done;
    }
    /* What stops --regen here, the later steps report: a tests.txt that
     * doesn't read, no ref, @gen without gen.c, a build that fails. */
    rc = lc_parse_regen_cases(ck->ctx, lc_str_cstr(&text), text.len,
                              ck->tests, &cases);
    if (rc != LC_OK) {
        cases = NULL;
        if (rc != LC_ERR_NOMEM)
            rc = bail(ck);
        goto done;
    }
    for (i = 0; i < cases->count; i++)
        any_gen |= cases->items[i].gen != NULL;
    if (!ck->ref || (any_gen && !ck->gen)) {
        rc = bail(ck);
        goto done;
    }
    rc = ensure_ref(ck);
    if (rc == LC_OK && any_gen)
        rc = ensure_gen(ck);
    if (rc == LC_OK && !ck->r->cancelled &&
        (!ck->ref_exe || (any_gen && !ck->gen_exe)))
        rc = bail(ck);
    if (rc != LC_OK || !ck->ref_exe || (any_gen && !ck->gen_exe))
        goto done;

    rg = calloc(cases->count ? cases->count : 1, sizeof *rg);
    made = calloc(cases->count ? cases->count : 1, sizeof *made);
    cwd = lc_dir_of(&ck->tmp, ck->ref_exe);
    if (!rg || !made || !cwd) {
        rc = lc_oom(ck->ctx);
        goto done;
    }
    for (i = 0; i < cases->count; i++) {
        lc_case c = cases->items[i];
        const lc_value *args, *exp;
        const char *now;
        lc_verdict v;
        lc_pool *pool;
        int stopped, keep;

        if (cancelled(ck)) {
            ck->r->cancelled = 1;
            goto done;
        }
        if (c.gen) {
            rc = gen_case(ck, c.gen, c.line, &made[i]);
            if (rc != LC_OK || !made[i])
                goto done;
            /* A bare @gen line has no input lines at all, not even "()". */
            if (!same_inputs(&c, &made[i]->items[0]) || c.last_line == 0) {
                rg[i].new_inputs = &made[i]->items[0];
                generated++;
            }
            c.args = made[i]->items[0].args;
            c.nargs = made[i]->items[0].nargs;
        }
        /* Every case must fit before ref runs on it. */
        c.expected = NULL;
        pool = lc_pool_new();
        if (!pool) {
            rc = lc_oom(ck->ctx);
            goto done;
        }
        rc = lc_case_values(ck->ctx, ck->p, &c, pool, &args, &exp);
        lc_pool_free(pool);
        if (rc != LC_OK && rc != LC_ERR_NOMEM && c.gen) {
            /* gen.c made a case that doesn't fit: only --regen sees it. */
            ck->gen_failed = 1;
            rc = error_finding(ck, LC_FINDING_ERROR, NULL);
            goto done;
        }
        if (rc != LC_OK) {
            rc = rc == LC_ERR_NOMEM ? rc : bail(ck);
            goto done;
        }
        rc = lc_run_case(ck->ctx, ck->p, ck->ref_exe, cwd, &c, &ck->quiet,
                         &ck->tmp, &now, &v, &why, &stopped);
        if (rc != LC_OK) {
            rc = ck->ctx->err_nomem ? LC_ERR_NOMEM
                                    : error_finding(ck, LC_FINDING_ERROR,
                                                    NULL);
            goto done;
        }
        if (stopped) {
            ck->r->cancelled = 1;
            goto done;
        }
        if (v != LC_VERDICT_NONE) {
            lc_str detail = LC_STR_INIT;

            /* A case that already has its result: checking ref on every
             * case reports it. */
            if (cases->items[i].expected && !rg[i].new_inputs) {
                rc = bail(ck);
                goto done;
            }
            case_detail(&detail, ck->p, c.args, c.nargs, NULL, NULL, NULL,
                        NULL);
            rc = detail.failed ? lc_oom(ck->ctx)
                               : finding(ck, LC_FINDING_ERROR, ck->tests,
                                         c.line, detail.data, "--regen: %s "
                                         "fails this case: %s (%s)",
                                         short_name(ck, ck->ref),
                                         lc_verdict_name(v),
                                         lc_str_cstr(&why));
            lc_str_free(&detail);
            goto done;
        }
        keep = rg[i].new_inputs ? 0 : still_matches(ck,
                                                     cases->items[i].expected,
                                                     now);
        if (keep < 0) {
            rc = lc_oom(ck->ctx);
            goto done;
        }
        if (!keep && !(cases->items[i].expected &&
                       strcmp(cases->items[i].expected, now) == 0)) {
            rg[i].expected = now;
            changed++;
        }
    }

    rc = compose(ck, lc_str_cstr(&text), text.len, cases, rg, &out);
    if (rc != LC_OK)
        goto done;
    ck->r->regen_changed = changed;
    ck->r->regen_generated = generated;
    if (out.len == text.len && memcmp(lc_str_cstr(&out), lc_str_cstr(&text),
                                      text.len) == 0) {
        rc = finding(ck, LC_FINDING_NOTE, ck->tests, 0, NULL, "--regen: "
                     "tests.txt is up to date");
        goto done;
    }
    rc = lc_write_file_atomic(ck->ctx, ck->tests, lc_str_cstr(&out), out.len);
    if (rc != LC_OK) {
        rc = ck->ctx->err_nomem ? LC_ERR_NOMEM
                                : error_finding(ck, LC_FINDING_ERROR, NULL);
        goto done;
    }
    ck->r->regen_written = 1;
    rc = finding(ck, LC_FINDING_NOTE, ck->tests, 0, NULL, "--regen: rewrote "
                 "tests.txt: %lu expected result%s, %lu generated case%s",
                 (unsigned long)changed, changed == 1 ? "" : "s",
                 (unsigned long)generated, generated == 1 ? "" : "s");
done:
    for (i = 0; made && cases && i < cases->count; i++)
        lc_cases_free(made[i]);
    free(made);
    free(rg);
    lc_cases_free(cases);
    lc_str_free(&text);
    lc_str_free(&out);
    lc_str_free(&why);
    return rc;
}

/* ------------------------------------------------------------------ */
/* 3. tests.txt                                                        */
/* ------------------------------------------------------------------ */

/* Loads tests.txt and checks every case; *cases is NULL when it can't be
 * read, and *bad is set when a case doesn't fit. */
static int check_tests(checker *ck, lc_cases **cases, int *bad)
{
    lc_pool *pool;
    size_t i;
    int rc;

    *bad = 0;
    rc = lc_load_cases(ck->ctx, ck->p, cases);
    if (rc != LC_OK) {
        *cases = NULL;
        return rc == LC_ERR_NOMEM ? rc : error_finding(ck, LC_FINDING_ERROR,
                                                       NULL);
    }
    pool = lc_pool_new();
    if (!pool)
        return lc_oom(ck->ctx);
    for (i = 0; i < (*cases)->count; i++) {
        const lc_case *c = &(*cases)->items[i];
        const lc_value *args, *exp;

        ck->r->ncases++;
        ck->r->nexamples += c->example != 0;
        ck->r->ngenerated += c->gen != NULL;
        rc = lc_case_values(ck->ctx, ck->p, c, pool, &args, &exp);
        if (rc == LC_ERR_NOMEM)
            break;
        if (rc != LC_OK) {
            *bad = 1;
            rc = error_finding(ck, LC_FINDING_ERROR, NULL);
            if (rc != LC_OK)
                break;
        }
        rc = LC_OK;
    }
    lc_pool_free(pool);
    if (rc != LC_OK)
        return rc;
    if ((*cases)->count == 0)
        return finding(ck, LC_FINDING_ERROR, ck->tests, 0, NULL, "%s", lc_is_file(
                           ck->tests) ? "tests.txt has no cases"
                                      : "there is no tests.txt");
    if (ck->r->nexamples == 0)
        return finding(ck, LC_FINDING_WARNING, ck->tests, 0, NULL, "no case "
                       "is marked @example, so the statement shows no "
                       "examples");
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* 4. The stubs                                                        */
/* ------------------------------------------------------------------ */

static int check_stubs(checker *ck)
{
    const char *cache = lc_cache_dir(ck->ctx);
    char *dir;
    size_t i;
    int rc;

    if (!cache)
        return ck->ctx->err_nomem ? LC_ERR_NOMEM : error_finding(
                                                       ck, LC_FINDING_ERROR,
                                                       NULL);
    dir = lc_arena_printf(&ck->tmp, "%s/check/%04d-%s", cache, ck->p->id,
                          ck->p->slug);
    if (!dir)
        return lc_oom(ck->ctx);
    rc = lc_mkdir_p(ck->ctx, dir, 0777);
    if (rc != LC_OK)
        return ck->ctx->err_nomem ? LC_ERR_NOMEM : rc;
    for (i = 0; i < lc_lang_count() && !ck->r->cancelled; i++) {
        const lc_lang *lang = lc_lang_at(i);
        lc_str stub = LC_STR_INIT;
        const char *exe;
        char *path;

        {
            lc_compiler cc;

            rc = lc_compiler_get(ck->ctx, lang, &ck->tmp, &cc);
            if (rc == LC_ERR_NOMEM || ck->ctx->err_nomem)
                return LC_ERR_NOMEM;
            if (rc != LC_OK) {
                rc = finding(ck, LC_FINDING_WARNING, NULL, 0, NULL, "the "
                             "%s stub was not built: %s", lang->title,
                             lc_last_error(ck->ctx));
                if (rc != LC_OK)
                    return rc;
                continue;
            }
        }
        path = lc_arena_printf(&ck->tmp, "%s/stub.%s", dir, lang->ext);
        if (!path || lc_stub(ck->ctx, ck->p, lang, &stub) != LC_OK) {
            lc_str_free(&stub);
            return lc_oom(ck->ctx);
        }
        rc = lc_write_file_atomic(ck->ctx, path, lc_str_cstr(&stub), stub.len);
        lc_str_free(&stub);
        if (rc != LC_OK)
            return ck->ctx->err_nomem ? LC_ERR_NOMEM : rc;
        rc = build(ck, lang, path, LC_ROLE_STUB,
                   lang->id == LC_LANG_C ? "the C stub" : "the C++ stub",
                   &exe);
        if (rc != LC_OK)
            return rc;
        if (exe)
            ck->r->stubs++;
    }
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* 5. The reference solution                                           */
/* ------------------------------------------------------------------ */

static int check_ref(checker *ck, const lc_cases *cases)
{
    const char *name = short_name(ck, ck->ref);
    lc_judge_opts jo;
    lc_report *rep = NULL;
    size_t i, shown = 0, slowest = 0;
    int rc;

    rc = ensure_ref(ck);
    if (rc != LC_OK || !ck->ref_exe || !cases->count)
        return rc;
    memset(&jo, 0, sizeof jo);
    jo.cwd = lc_dir_of(&ck->tmp, ck->ref_exe);
    if (!jo.cwd)
        return lc_oom(ck->ctx);
    rc = lc_judge_exe(ck->ctx, ck->p, ck->ref_exe, cases->items, cases->count,
                      &jo, &ck->quiet, &rep);
    if (rc != LC_OK)
        return ck->ctx->err_nomem ? LC_ERR_NOMEM
                                  : error_finding(ck, LC_FINDING_ERROR, NULL);
    if (rep->cancelled) {
        ck->r->cancelled = 1;
        lc_report_free(rep);
        return LC_OK;
    }
    ck->r->ref_passed = rep->passed;
    ck->r->ref_max_ms = rep->max_time_ms;
    for (i = 0; i < rep->nresults && rc == LC_OK; i++) {
        const lc_case_result *res = &rep->results[i];
        lc_str detail = LC_STR_INIT;

        if (res->time_ms > rep->results[slowest].time_ms)
            slowest = i;
        if (res->verdict == LC_ACCEPTED || shown++ >= MAX_CASE_FINDINGS)
            continue;
        case_detail(&detail, ck->p, res->inputs, res->ninputs, "Expected:",
                    res->expected, "Output:", res->output);
        if (detail.failed) {
            rc = lc_oom(ck->ctx);
        } else if (res->message) {
            lc_str why = LC_STR_INIT;

            lc_str_puts(&why, res->message);
            lc_add_harness_reason(&why, res->exit_code, res->stderr_text);
            rc = why.failed ? lc_oom(ck->ctx)
                            : finding(ck, LC_FINDING_ERROR, ck->tests,
                                      res->line, detail.data, "%s gets %s "
                                      "on this case (%s)", name,
                                      lc_verdict_name(res->verdict),
                                      why.data);
            lc_str_free(&why);
        } else {
            rc = finding(ck, LC_FINDING_ERROR, ck->tests, res->line,
                         detail.data, "%s gets %s on this case", name,
                         lc_verdict_name(res->verdict));
        }
        lc_str_free(&detail);
    }
    if (rc == LC_OK && shown > MAX_CASE_FINDINGS)
        rc = finding(ck, LC_FINDING_ERROR, ck->tests, 0, NULL, "%s fails %lu "
                     "more cases", name,
                     (unsigned long)(shown - MAX_CASE_FINDINGS));
    /* Room for slower machines and other solutions (docs/PLAN.md, "Timing
     * noise"): the limit should be about three times ref's slowest case. */
    if (rc == LC_OK && rep->nresults &&
        rep->max_time_ms * TIME_SHARE > ck->p->time_limit_ms)
        rc = finding(ck, LC_FINDING_WARNING, ck->tests,
                     rep->results[slowest].line, NULL, "%s takes %ld ms on "
                     "this case, more than a third of the time limit (%d ms)",
                     name, rep->max_time_ms, ck->p->time_limit_ms);
    lc_report_free(rep);
    return rc;
}

/* ------------------------------------------------------------------ */
/* 6. @gen cases are up to date                                        */
/* ------------------------------------------------------------------ */

static int check_gen_cases(checker *ck, const lc_cases *cases)
{
    size_t i;
    int rc;

    for (i = 0; i < cases->count; i++) {
        const lc_case *c = &cases->items[i];
        lc_cases *made;

        if (!c->gen)
            continue;
        if (!ck->gen) {
            rc = finding(ck, LC_FINDING_ERROR, ck->tests, c->line, NULL,
                         "@gen needs the pack's gen.c, which makes this case");
            if (rc != LC_OK)
                return rc;
            continue;
        }
        rc = ensure_gen(ck);
        if (rc != LC_OK || !ck->gen_exe || ck->r->cancelled ||
            ck->gen_failed)
            return rc;
        rc = gen_case(ck, c->gen, c->line, &made);
        if (rc != LC_OK || ck->r->cancelled)
            return rc;
        if (made && !same_inputs(c, &made->items[0]))
            rc = finding(ck, LC_FINDING_ERROR, ck->tests, c->line, NULL,
                         "this case is not what gen.c makes for '@gen %s' "
                         "now; run 'lc check --regen' to make it again",
                         c->gen);
        lc_cases_free(made);
        if (rc != LC_OK)
            return rc;
    }
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* 7. ref and brute agree                                              */
/* ------------------------------------------------------------------ */

static int cross_check(checker *ck, const lc_check_options *opts)
{
    unsigned long long count = opts->count ? opts->count : LC_CHECK_COUNT;
    unsigned long long seed = opts->seed, s;
    const char *brute = short_name(ck, ck->brute);
    const char *ref = short_name(ck, ck->ref);
    lc_str why = LC_STR_INIT, detail = LC_STR_INIT, about = LC_STR_INIT;
    lc_cases *made = NULL;
    lc_case *cases = NULL;
    lc_judge_opts jo;
    lc_report *rep = NULL;
    const char *bdir;
    size_t i;
    int rc;

    if (!ck->gen)
        return finding(ck, LC_FINDING_ERROR, ck->brute, 0, NULL, "%s needs "
                       "gen.c, which makes the random cases to compare it "
                       "with %s on", brute, ref);
    rc = ensure_gen(ck);
    if (rc == LC_OK)
        rc = ensure_brute(ck);
    if (rc == LC_OK)
        rc = ensure_ref(ck);
    if (rc != LC_OK || !ck->gen_exe || !ck->brute_exe || !ck->ref_exe ||
        ck->r->cancelled || ck->gen_failed)
        return rc;
    if (lc_str_printf(&about, "the seeds %llu to %llu (small)", seed,
                      seed + count - 1) != LC_OK)
        return lc_oom(ck->ctx);
    rc = run_gen(ck, seed, 1, count, ck->gen, 0, about.data, &made);
    if (rc != LC_OK || !made)
        goto done;
    if (made->count != count) {
        rc = finding(ck, LC_FINDING_ERROR, ck->gen, 0, NULL, "gen.c printed "
                     "%lu cases for %llu seeds: each call of gen() must "
                     "print one case, with no blank lines in it",
                     (unsigned long)made->count, count);
        goto done;
    }
    cases = lc_arena_array(&ck->tmp, made->count, sizeof *cases);
    bdir = lc_dir_of(&ck->tmp, ck->brute_exe);
    if (!cases || !bdir) {
        rc = lc_oom(ck->ctx);
        goto done;
    }
    for (i = 0; i < made->count; i++) {
        lc_pool *pool = lc_pool_new();
        const lc_value *args, *exp;
        lc_verdict v;
        int stopped;

        s = seed + i;
        cases[i] = made->items[i];
        cases[i].file = ck->gen;
        cases[i].line = 0;
        if (!pool) {
            rc = lc_oom(ck->ctx);
            goto done;
        }
        rc = lc_case_values(ck->ctx, ck->p, &cases[i], pool, &args, &exp);
        lc_pool_free(pool);
        if (rc == LC_ERR_NOMEM)
            goto done;
        if (rc != LC_OK) {
            case_detail(&detail, ck->p, cases[i].args, cases[i].nargs, NULL,
                        NULL, NULL, NULL);
            rc = finding(ck, LC_FINDING_ERROR, ck->gen, 0, detail.data,
                         "gen.c makes a case that doesn't fit the signature "
                         "for seed %llu (small): %s", s,
                         lc_last_error(ck->ctx));
            goto done;
        }
        rc = lc_run_case(ck->ctx, ck->p, ck->brute_exe, bdir, &cases[i],
                         &ck->quiet, &ck->tmp, &cases[i].expected, &v, &why,
                         &stopped);
        if (rc != LC_OK) {
            rc = ck->ctx->err_nomem ? LC_ERR_NOMEM
                                    : error_finding(ck, LC_FINDING_ERROR,
                                                    NULL);
            goto done;
        }
        if (stopped) {
            ck->r->cancelled = 1;
            goto done;
        }
        if (v != LC_VERDICT_NONE) {
            case_detail(&detail, ck->p, cases[i].args, cases[i].nargs, NULL,
                        NULL, NULL, NULL);
            rc = detail.failed ? lc_oom(ck->ctx)
                               : finding(ck, LC_FINDING_ERROR, ck->brute, 0,
                                         detail.data, "%s gets %s on the "
                                         "random case for seed %llu (%s)",
                                         brute, lc_verdict_name(v), s,
                                         lc_str_cstr(&why));
            goto done;
        }
    }

    memset(&jo, 0, sizeof jo);
    jo.cwd = lc_dir_of(&ck->tmp, ck->ref_exe);
    jo.stop_on_failure = 1;
    if (!jo.cwd) {
        rc = lc_oom(ck->ctx);
        goto done;
    }
    rc = lc_judge_exe(ck->ctx, ck->p, ck->ref_exe, cases, made->count, &jo,
                      &ck->quiet, &rep);
    if (rc != LC_OK) {
        rc = ck->ctx->err_nomem ? LC_ERR_NOMEM
                                : error_finding(ck, LC_FINDING_ERROR, NULL);
        goto done;
    }
    if (rep->cancelled) {
        ck->r->cancelled = 1;
        goto done;
    }
    ck->r->cross = rep->passed;
    if (rep->verdict != LC_ACCEPTED && rep->nresults) {
        const lc_case_result *res = &rep->results[rep->nresults - 1];

        s = seed + res->index;
        lc_str_clear(&detail);
        if (res->verdict == LC_WRONG_ANSWER) {
            char l1[64], l2[64];

            snprintf(l1, sizeof l1, "%s:", ref);
            snprintf(l2, sizeof l2, "%s:", brute);
            case_detail(&detail, ck->p, res->inputs, res->ninputs, l1,
                        res->output, l2, res->expected);
            rc = detail.failed ? lc_oom(ck->ctx)
                               : finding(ck, LC_FINDING_ERROR, ck->ref, 0,
                                         detail.data, "%s and %s disagree "
                                         "on the random case for seed %llu "
                                         "(small)", ref, brute, s);
        } else {
            case_detail(&detail, ck->p, res->inputs, res->ninputs, NULL,
                        NULL, NULL, NULL);
            lc_str_clear(&why);
            lc_str_puts(&why, res->message ? res->message : "");
            lc_add_harness_reason(&why, res->exit_code, res->stderr_text);
            rc = detail.failed || why.failed
                     ? lc_oom(ck->ctx)
                     : finding(ck, LC_FINDING_ERROR, ck->ref, 0, detail.data,
                               "%s gets %s on the random case for seed %llu "
                               "(%s)", ref, lc_verdict_name(res->verdict), s,
                               lc_str_cstr(&why));
        }
    }
done:
    lc_report_free(rep);
    lc_cases_free(made);
    lc_str_free(&why);
    lc_str_free(&detail);
    lc_str_free(&about);
    return rc;
}

/* ------------------------------------------------------------------ */
/* lc_check                                                            */
/* ------------------------------------------------------------------ */

/* Finds a pack source ("ref", "brute"): an error when there are both. */
static int find_source(checker *ck, const char *base, char **out,
                       const lc_lang **lang)
{
    int both;

    *out = lc_pack_source(&ck->tmp, ck->p, base, lang, &both);
    if (both)
        return finding(ck, LC_FINDING_ERROR, NULL, 0, NULL, "the pack has "
                       "both %s.c and %s.cpp; keep one (lc uses %s)", base,
                       base, short_name(ck, *out));
    return LC_OK;
}

int lc_check(lc_ctx *ctx, const lc_problem *p, const lc_manifest *m,
             const lc_check_options *opts, const lc_hooks *hooks,
             lc_check_report **out)
{
    lc_check_options defaults;
    lc_cases *cases = NULL;
    checker ck;
    int rc, bad = 0, no_result = 0;

    if (!out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_check: bad arguments");
    *out = NULL;
    if (!p)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_check: bad arguments");
    if (!opts) {
        memset(&defaults, 0, sizeof defaults);
        opts = &defaults;
    }
    memset(&ck, 0, sizeof ck);
    ck.ctx = ctx;
    ck.p = p;
    ck.hooks = hooks;
    ck.quiet.cancel = hooks ? hooks->cancel : NULL;
    ck.box = calloc(1, sizeof *ck.box);
    if (!ck.box)
        return lc_oom(ctx);
    ck.r = &ck.box->pub;
    ck.r->priv_ = ck.box;
    ck.problem_md = lc_path_join(&ck.tmp, p->dir, "problem.md");
    ck.tests = lc_path_join(&ck.tmp, p->dir, "tests.txt");
    if (!ck.problem_md || !ck.tests) {
        rc = lc_oom(ctx);
        goto done;
    }

    rc = check_manifest(&ck, m);
    if (rc == LC_OK && p->statement_len == 0)
        rc = finding(&ck, LC_FINDING_WARNING, ck.problem_md, 0, NULL,
                     "the statement is empty");
    if (rc == LC_OK)
        rc = find_source(&ck, "ref", &ck.ref, &ck.ref_lang);
    if (rc == LC_OK)
        rc = find_source(&ck, "brute", &ck.brute, &ck.brute_lang);
    if (rc != LC_OK)
        goto done;
    ck.gen = lc_path_join(&ck.tmp, p->dir, "gen.c");
    if (!ck.gen) {
        rc = lc_oom(ctx);
        goto done;
    }
    if (!lc_is_file(ck.gen))
        ck.gen = NULL;
    ck.r->ref = ck.ref ? lc_arena_strdup(&ck.box->arena, short_name(&ck, ck.ref))
                       : NULL;
    ck.r->brute = ck.brute ? lc_arena_strdup(&ck.box->arena,
                                             short_name(&ck, ck.brute))
                           : NULL;
    ck.r->gen = ck.gen ? "gen.c" : NULL;
    if ((ck.ref && !ck.r->ref) || (ck.brute && !ck.r->brute)) {
        rc = lc_oom(ctx);
        goto done;
    }

    /* Whether lc can judge this problem at all yet. A function that
     * returns void with nothing to check is not that but a mistake in
     * the pack: an error, and nothing can be run either. */
    if (lc_codegen_supported(ctx, p) != LC_OK) {
        ck.r->skipped = lc_arena_strdup(&ck.box->arena, lc_last_error(ctx));
        if (!ck.r->skipped) {
            rc = lc_oom(ctx);
            goto done;
        }
    } else if (lc_result_type(p)->kind == LC_T_VOID) {
        char why[LC_VOID_REASON_MAX];

        lc_codegen_void_reason(p, why);
        no_result = 1;
        rc = finding(&ck, LC_FINDING_ERROR, ck.problem_md,
                     field_line(p, "signature"), NULL, "%s", why);
        if (rc != LC_OK)
            goto done;
    }
    if (opts->regen && (ck.r->skipped || no_result))
        rc = finding(&ck, LC_FINDING_NOTE, ck.tests, 0, NULL, "--regen: "
                     "tests.txt was not rewritten, as nothing can be "
                     "run%s", no_result ? "" : " yet");
    else if (opts->regen)
        rc = regen(&ck);
    if (rc != LC_OK || ck.r->cancelled)
        goto done;

    rc = check_tests(&ck, &cases, &bad);
    if (rc != LC_OK)
        goto done;
    if (ck.r->skipped) {
        rc = finding(&ck, LC_FINDING_NOTE, NULL, 0, NULL, "nothing was "
                     "built or run: %s", ck.r->skipped);
        goto done;
    }
    if (no_result)
        goto done;
    rc = check_stubs(&ck);
    if (rc == LC_OK && !ck.ref)
        rc = finding(&ck, LC_FINDING_ERROR, NULL, 0, NULL, "no reference "
                     "solution: write ref.c or ref.cpp");
    else if (rc == LC_OK && cases && !bad && !ck.r->cancelled)
        rc = check_ref(&ck, cases);
    if (rc == LC_OK && cases && !ck.r->cancelled)
        rc = check_gen_cases(&ck, cases);
    if (rc == LC_OK && ck.brute && ck.ref && !ck.r->cancelled)
        rc = cross_check(&ck, opts);
    if (rc == LC_OK && ck.gen && !ck.brute && !ck.r->ngenerated)
        rc = finding(&ck, LC_FINDING_WARNING, ck.gen, 0, NULL, "gen.c is not "
                     "used: add brute.c to compare ref with on random "
                     "cases, or @gen cases to tests.txt");
done:
    lc_cases_free(cases);
    lc_arena_free(&ck.tmp);
    if (rc != LC_OK) {
        lc_check_report_free(ck.r);
        return rc;
    }
    ck.r->ok = ck.r->errors == 0;
    *out = ck.r;
    return LC_OK;
}
