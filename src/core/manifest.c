#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * manifest.c: MANIFEST.tsv, the list of LeetCode problems in scope and
 * how far each one's pack is (docs/authoring.md, "MANIFEST.tsv"):
 *
 *   # id  slug     title    difficulty  premium  group  status
 *   1     two-sum  Two Sum  easy        no       plain  verified
 *
 * Fields are separated by single tabs. Lines starting with '#' and blank
 * lines are ignored. Rows must be in increasing id order, so every id is
 * listed once and the file diffs well.
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "internal.h"

#define MANIFEST_FILE "MANIFEST.tsv"
#define MAX_MANIFEST (16 * LC_MIB)
#define MAX_ID 99999
#define NFIELDS 7

/* The groups of docs/PLAN.md section 2.2, one per harness feature. */
static const char *const groups[] = {
    "plain",    "bits", "float", "inplace", "unordered", "list",
    "list-builder", "tree", "tree-ref", "nary", "design", "api",
    "checker",  "extend", NULL,
};

static const char *const statuses[] = {"todo", "draft", "verified", NULL};

typedef struct manifest_box {
    lc_manifest pub;          /* first: a pointer to it is one to the box */
    lc_arena arena;
} manifest_box;

typedef struct field {
    const char *p;
    size_t n;
} field;

static int manifest_error(lc_ctx *ctx, const char *path, int line,
                          const char *fmt, ...) LC_PRINTF(4, 5);

static int manifest_error(lc_ctx *ctx, const char *path, int line,
                          const char *fmt, ...)
{
    lc_str msg = LC_STR_INIT;
    va_list ap;

    lc_str_printf(&msg, "%s:%d: ", path, line);
    va_start(ap, fmt);
    lc_str_vprintf(&msg, fmt, ap);
    va_end(ap);
    if (msg.failed) {
        lc_str_free(&msg);
        return lc_oom(ctx);
    }
    lc_set_error(ctx, LC_ERR_PARSE, "%s", msg.data);
    lc_str_free(&msg);
    return ctx->err_nomem ? LC_ERR_NOMEM : LC_ERR_PARSE;
}

static int field_is(field f, const char *s)
{
    return f.n == strlen(s) && memcmp(f.p, s, f.n) == 0;
}

static const char *find_word(const char *const *list, field f)
{
    for (; *list; list++)
        if (field_is(f, *list))
            return *list;
    return NULL;
}

/* lowercase letters and digits in groups joined by single dashes */
static int valid_slug(field f)
{
    size_t i;

    if (f.n == 0 || f.p[0] == '-' || f.p[f.n - 1] == '-')
        return 0;
    for (i = 0; i < f.n; i++) {
        char c = f.p[i];
        if (c == '-' && f.p[i + 1] == '-')
            return 0;
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-'))
            return 0;
    }
    return 1;
}

static int parse_id(field f)
{
    long v = 0;
    size_t i;

    if (f.n == 0 || f.n > 5 || f.p[0] == '0')
        return 0;
    for (i = 0; i < f.n; i++) {
        if (f.p[i] < '0' || f.p[i] > '9')
            return 0;
        v = v * 10 + (f.p[i] - '0');
    }
    return v <= MAX_ID ? (int)v : 0;
}

/* Splits at tabs into at most `max` fields; returns how many there were
 * (more than `max` if there are too many). */
static size_t split(const char *p, size_t n, field *f, size_t max)
{
    size_t k = 0;

    for (;;) {
        const char *t = memchr(p, '\t', n);
        size_t len = t ? (size_t)(t - p) : n;

        if (k < max) {
            f[k].p = p;
            f[k].n = len;
        }
        k++;
        if (!t)
            return k;
        p = t + 1;
        n -= len + 1;
    }
}

static int cmp_slug(const void *a, const void *b)
{
    const lc_manifest_row *x = *(const lc_manifest_row *const *)a;
    const lc_manifest_row *y = *(const lc_manifest_row *const *)b;
    int c = strcmp(x->slug, y->slug);

    return c ? c : (x->line > y->line) - (x->line < y->line);
}

int lc_parse_manifest(lc_ctx *ctx, const char *text, size_t len,
                      const char *path, lc_manifest **out)
{
    manifest_box *box;
    lc_manifest_row *rows = NULL, *r;
    size_t n = 0, cap = 0, nf, i;
    const char *p = text ? text : "", *end = p + len;
    int lineno = 0, rc = LC_OK;

    if (!out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_parse_manifest: bad "
                            "arguments");
    *out = NULL;
    if (!path)
        path = MANIFEST_FILE;
    box = calloc(1, sizeof *box);
    if (!box)
        return lc_oom(ctx);
    box->pub.path = lc_arena_strdup(&box->arena, path);
    if (!box->pub.path) {
        rc = lc_oom(ctx);
        goto fail;
    }
    if (len >= 3 && memcmp(p, "\xEF\xBB\xBF", 3) == 0)
        p += 3;
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *ls = p, *le = nl ? nl : end;
        field f[NFIELDS];
        const char *word;

        p = nl ? nl + 1 : end;
        lineno++;
        if (le > ls && le[-1] == '\r')
            le--;
        if (ls == le || *ls == '#')
            continue;
        if (memchr(ls, '\0', (size_t)(le - ls))) {
            rc = manifest_error(ctx, path, lineno, "line contains a NUL byte");
            goto fail;
        }
        nf = split(ls, (size_t)(le - ls), f, NFIELDS);
        if (nf != NFIELDS) {
            rc = manifest_error(ctx, path, lineno, "expected %d "
                                "tab-separated fields (id, slug, title, "
                                "difficulty, premium, group, status), got "
                                "%lu", NFIELDS, (unsigned long)nf);
            goto fail;
        }
        {
            lc_manifest_row *tmp = lc_grow(rows, &cap, n + 1, sizeof *rows);
            if (!tmp) {
                rc = lc_oom(ctx);
                goto fail;
            }
            rows = tmp;
        }
        r = &rows[n];
        memset(r, 0, sizeof *r);
        r->line = lineno;
        r->id = parse_id(f[0]);
        if (!r->id) {
            rc = manifest_error(ctx, path, lineno, "invalid id '%.*s' "
                                "(expected an integer from 1 to 99999)",
                                (int)(f[0].n > 60 ? 60 : f[0].n), f[0].p);
            goto fail;
        }
        if (n > 0 && r->id <= rows[n - 1].id) {
            rc = manifest_error(ctx, path, lineno, r->id == rows[n - 1].id
                                ? "id %d is listed twice (rows must be in "
                                  "increasing id order)"
                                : "id %d is out of order (rows must be in "
                                  "increasing id order)", r->id);
            goto fail;
        }
        if (!valid_slug(f[1])) {
            rc = manifest_error(ctx, path, lineno, "invalid slug '%.*s'",
                                (int)(f[1].n > 60 ? 60 : f[1].n), f[1].p);
            goto fail;
        }
        if (f[2].n == 0) {
            rc = manifest_error(ctx, path, lineno, "empty title");
            goto fail;
        }
        r->difficulty = LC_DIFF_UNKNOWN;
        if (field_is(f[3], "easy"))
            r->difficulty = LC_EASY;
        else if (field_is(f[3], "medium"))
            r->difficulty = LC_MEDIUM;
        else if (field_is(f[3], "hard"))
            r->difficulty = LC_HARD;
        if (r->difficulty == LC_DIFF_UNKNOWN) {
            rc = manifest_error(ctx, path, lineno, "invalid difficulty "
                                "'%.*s' (expected easy, medium or hard)",
                                (int)(f[3].n > 60 ? 60 : f[3].n), f[3].p);
            goto fail;
        }
        if (field_is(f[4], "yes") || field_is(f[4], "no")) {
            r->premium = field_is(f[4], "yes");
        } else {
            rc = manifest_error(ctx, path, lineno, "invalid premium '%.*s' "
                                "(expected yes or no)",
                                (int)(f[4].n > 60 ? 60 : f[4].n), f[4].p);
            goto fail;
        }
        if (f[5].n > 5 && memcmp(f[5].p, "skip:", 5) == 0) {
            field reason;

            reason.p = f[5].p + 5;
            reason.n = f[5].n - 5;
            if (!valid_slug(reason)) {
                rc = manifest_error(ctx, path, lineno, "invalid skip reason "
                                    "'%.*s' (use lowercase letters, digits "
                                    "and dashes)",
                                    (int)(reason.n > 60 ? 60 : reason.n),
                                    reason.p);
                goto fail;
            }
            r->group = "skip";
            r->skip = lc_arena_strndup(&box->arena, reason.p, reason.n);
            if (!r->skip) {
                rc = lc_oom(ctx);
                goto fail;
            }
        } else if ((word = find_word(groups, f[5])) != NULL) {
            r->group = word;
        } else {
            rc = manifest_error(ctx, path, lineno, "unknown group '%.*s' "
                                "(expected one of plain, bits, float, "
                                "inplace, unordered, list, list-builder, "
                                "tree, tree-ref, nary, design, api, checker, "
                                "extend, or skip:REASON)",
                                (int)(f[5].n > 60 ? 60 : f[5].n), f[5].p);
            goto fail;
        }
        word = find_word(statuses, f[6]);
        if (!word) {
            rc = manifest_error(ctx, path, lineno, "invalid status '%.*s' "
                                "(expected todo, draft or verified)",
                                (int)(f[6].n > 60 ? 60 : f[6].n), f[6].p);
            goto fail;
        }
        r->status = word;
        r->slug = lc_arena_strndup(&box->arena, f[1].p, f[1].n);
        r->title = lc_arena_strndup(&box->arena, f[2].p, f[2].n);
        if (!r->slug || !r->title) {
            rc = lc_oom(ctx);
            goto fail;
        }
        n++;
    }
    /* Slugs must be unique too: sort them, then look at neighbours. */
    if (n > 1) {
        const lc_manifest_row **by = malloc(n * sizeof *by);

        if (!by) {
            rc = lc_oom(ctx);
            goto fail;
        }
        for (i = 0; i < n; i++)
            by[i] = &rows[i];
        qsort(by, n, sizeof *by, cmp_slug);
        for (i = 1; i < n; i++)
            if (strcmp(by[i - 1]->slug, by[i]->slug) == 0)
                break;
        if (i < n) {
            const lc_manifest_row *a = by[i - 1], *b = by[i];

            if (a->line > b->line) {
                a = by[i];
                b = by[i - 1];
            }
            rc = manifest_error(ctx, path, b->line, "slug '%s' is already "
                                "used on line %d", b->slug, a->line);
            free(by);
            goto fail;
        }
        free(by);
    }
    {
        lc_manifest_row *final = lc_arena_array(&box->arena, n ? n : 1,
                                                sizeof *final);
        if (!final) {
            rc = lc_oom(ctx);
            goto fail;
        }
        if (n)
            memcpy(final, rows, n * sizeof *final);
        box->pub.rows = final;
        box->pub.count = n;
    }
    box->pub.priv_ = box;
    free(rows);
    *out = &box->pub;
    return LC_OK;
fail:
    free(rows);
    lc_arena_free(&box->arena);
    free(box);
    return rc;
}

int lc_load_manifest(lc_ctx *ctx, lc_manifest **out)
{
    lc_str path = LC_STR_INIT, text = LC_STR_INIT;
    struct stat st;
    int rc;

    if (!out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_load_manifest: bad "
                            "arguments");
    *out = NULL;
    if (!ctx->loaded)
        return lc_set_error(ctx, LC_ERR_INVALID, "problems are not loaded");
    if (lc_str_printf(&path, "%s/%s", ctx->problems_dir, MANIFEST_FILE) !=
        LC_OK) {
        lc_str_free(&path);
        return lc_oom(ctx);
    }
    if (stat(path.data, &st) != 0 && (errno == ENOENT || errno == ENOTDIR)) {
        lc_str_free(&path);
        return LC_OK;
    }
    rc = lc_read_file(ctx, path.data, MAX_MANIFEST, &text);
    if (rc == LC_OK)
        rc = lc_parse_manifest(ctx, lc_str_cstr(&text), text.len, path.data,
                               out);
    else if (ctx->err_nomem)
        rc = LC_ERR_NOMEM;
    lc_str_free(&path);
    lc_str_free(&text);
    return rc;
}

void lc_manifest_free(lc_manifest *m)
{
    manifest_box *box = (manifest_box *)m;

    if (!box)
        return;
    lc_arena_free(&box->arena);
    free(box);
}

const lc_manifest_row *lc_manifest_find(const lc_manifest *m, int id)
{
    size_t lo = 0, hi;

    if (!m)
        return NULL;
    hi = m->count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (m->rows[mid].id == id)
            return &m->rows[mid];
        if (m->rows[mid].id < id)
            lo = mid + 1;
        else
            hi = mid;
    }
    return NULL;
}
