#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * store.c: your progress and submission history (docs/workspace.md,
 * "Progress and history"), in the data directory:
 *
 *   ~/.local/share/lcclone/
 *     progress.tsv                     one line per problem you submitted
 *     progress.lock                    taken while progress.tsv is updated
 *     submissions/0001/
 *       20261030T201503Z-accepted.cpp  a copy of each submission
 *
 * progress.tsv is tab-separated: id, status, lang, best_ms, last_ts, with
 * "-" for an empty field. lc rewrites only the line of the problem it
 * records, so comments, lines it can't read and columns added by a later
 * version are kept.
 */

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "internal.h"

#define PROGRESS_FILE "progress.tsv"
#define LOCK_FILE "progress.lock"
#define MAX_PROGRESS (64 * LC_MIB)
#define MAX_ID 99999
/* Largest solution copied into the history. */
#define MAX_SOURCE (16 * LC_MIB)

static const char header[] =
    "# lcclone progress: one line per problem you have submitted.\n"
    "# id\tstatus\tlang\tbest_ms\tlast_ts\n";

const char *lc_status_name(lc_status s)
{
    switch (s) {
    case LC_ATTEMPTED:
        return "attempted";
    case LC_SOLVED:
        return "solved";
    default:
        return "todo";
    }
}

int lc_status_from_name(const char *s)
{
    if (!s)
        return -1;
    if (strcmp(s, "todo") == 0)
        return LC_TODO;
    if (strcmp(s, "attempted") == 0)
        return LC_ATTEMPTED;
    if (strcmp(s, "solved") == 0)
        return LC_SOLVED;
    return -1;
}

static void empty_entry(int id, lc_progress_entry *e)
{
    memset(e, 0, sizeof *e);
    e->id = id;
    e->status = LC_TODO;
    e->lang = "";
    e->best_ms = -1;
}

/* ------------------------------------------------------------------ */
/* Reading progress.tsv                                                */
/* ------------------------------------------------------------------ */

typedef struct field {
    const char *p;
    size_t n;
} field;

/* Splits a line at tabs into at most `max` fields; returns how many. */
static size_t split_tabs(const char *p, size_t n, field *f, size_t max)
{
    size_t k = 0;

    while (k < max) {
        const char *t = memchr(p, '\t', n);
        size_t len = t ? (size_t)(t - p) : n;

        f[k].p = p;
        f[k].n = len;
        k++;
        if (!t)
            break;
        p = t + 1;
        n -= len + 1;
    }
    return k;
}

static int field_is(field f, const char *s)
{
    return f.n == strlen(s) && memcmp(f.p, s, f.n) == 0;
}

/* A decimal number of at most `max`; -1 when the field isn't one. */
static long long parse_num(field f, long long max)
{
    long long v = 0;
    size_t i;

    if (f.n == 0 || f.n > 19)
        return -1;
    for (i = 0; i < f.n; i++) {
        int d = (unsigned char)f.p[i] - '0';
        if (d < 0 || d > 9 || v > (max - d) / 10)
            return -1;
        v = v * 10 + d;
    }
    return v;
}

/*
 * Parses one line of progress.tsv. Returns 1 for an entry (in *e), 0 for
 * a comment or blank line, and -1 for a line that can't be used, with the
 * reason in *why.
 */
static int parse_line(const char *p, size_t n, lc_progress_entry *e,
                      const char **why)
{
    field f[6];
    size_t nf;
    long long v;
    int st;
    const lc_lang *lang;

    if (n > 0 && p[n - 1] == '\r')
        n--;
    if (n == 0 || p[0] == '#')
        return 0;
    nf = split_tabs(p, n, f, 6);
    if (nf < 5) {
        *why = "expected 5 tab-separated fields";
        return -1;
    }
    v = parse_num(f[0], MAX_ID);
    if (v <= 0) {
        *why = "bad id";
        return -1;
    }
    empty_entry((int)v, e);
    st = -1;
    if (field_is(f[1], "attempted"))
        st = LC_ATTEMPTED;
    else if (field_is(f[1], "solved"))
        st = LC_SOLVED;
    else if (field_is(f[1], "todo"))
        st = LC_TODO;
    if (st < 0) {
        *why = "bad status (expected attempted or solved)";
        return -1;
    }
    e->status = (lc_status)st;
    if (!field_is(f[2], "-")) {
        lang = NULL;
        if (field_is(f[2], "c"))
            lang = lc_lang_find("c");
        else if (field_is(f[2], "cpp"))
            lang = lc_lang_find("cpp");
        if (!lang) {
            *why = "bad lang (expected c, cpp or -)";
            return -1;
        }
        e->lang = lang->name;
    }
    if (!field_is(f[3], "-")) {
        v = parse_num(f[3], LONG_MAX);
        if (v < 0) {
            *why = "bad best_ms";
            return -1;
        }
        e->best_ms = (long)v;
    }
    if (!field_is(f[4], "-")) {
        v = parse_num(f[4], LLONG_MAX);
        if (v < 0) {
            *why = "bad last_ts";
            return -1;
        }
        e->last_ts = v;
    }
    return 1;
}

/* An entry and the line it came from, for sorting. */
typedef struct numbered {
    lc_progress_entry e;
    size_t pos;
} numbered;

static int cmp_numbered(const void *a, const void *b)
{
    const numbered *x = a, *y = b;

    if (x->e.id != y->e.id)
        return (x->e.id > y->e.id) - (x->e.id < y->e.id);
    return (x->pos > y->pos) - (x->pos < y->pos);
}

/*
 * Parses the text of progress.tsv into a malloc'ed array sorted by id.
 * When a problem has several lines, the last one counts. With `warn`, bad
 * lines are recorded as warnings naming `path`.
 */
static int parse_progress(lc_ctx *ctx, const char *path, const char *text,
                          size_t len, int warn, lc_progress_entry **out,
                          size_t *count, size_t *cap)
{
    const char *p = text, *end = text + len;
    numbered *num = NULL;
    lc_progress_entry *arr, e;
    size_t n = 0, c = 0, i, k;
    int lineno = 0, r;

    *out = NULL;
    *count = *cap = 0;
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *le = nl ? nl : end, *why = NULL;

        lineno++;
        r = parse_line(p, (size_t)(le - p), &e, &why);
        p = nl ? nl + 1 : end;
        if (r < 0 && warn &&
            lc_add_warning(ctx, "%s:%d: %s; line skipped", path, lineno,
                           why) != LC_OK) {
            free(num);
            return LC_ERR_NOMEM;
        }
        if (r <= 0)
            continue;
        {
            numbered *tmp = lc_grow(num, &c, n + 1, sizeof *num);
            if (!tmp) {
                free(num);
                return lc_oom(ctx);
            }
            num = tmp;
        }
        num[n].e = e;
        num[n].pos = n;
        n++;
    }
    if (n > 1)
        qsort(num, n, sizeof *num, cmp_numbered);
    arr = malloc((n ? n : 1) * sizeof *arr);
    if (!arr) {
        free(num);
        return lc_oom(ctx);
    }
    for (i = 0, k = 0; i < n; i++)
        if (i + 1 == n || num[i + 1].e.id != num[i].e.id)
            arr[k++] = num[i].e;
    free(num);
    *out = arr;
    *count = k;
    *cap = n ? n : 1;
    return LC_OK;
}

static char *progress_path(lc_ctx *ctx, lc_arena *a)
{
    const char *dir = lc_data_dir(ctx);

    if (!dir)
        return NULL;
    return lc_path_join(a, dir, PROGRESS_FILE);
}

/* Reads progress.tsv into `text`; a missing file is empty. */
static int read_progress(lc_ctx *ctx, const char *path, lc_str *text)
{
    int rc = lc_read_file(ctx, path, MAX_PROGRESS, text);

    if (rc == LC_ERR_NOTFOUND) {
        lc_str_clear(text);
        return LC_OK;
    }
    return ctx->err_nomem ? LC_ERR_NOMEM : rc;
}

/*
 * Reads progress.tsv into the context, once. A file that can't be read
 * leaves no progress, with a warning: only running out of memory fails.
 */
static int load_progress(lc_ctx *ctx)
{
    lc_arena a = {NULL};
    lc_str text = LC_STR_INIT;
    lc_progress_entry *arr = NULL;
    size_t n = 0, cap = 0;
    char *path;
    int rc = LC_OK;

    if (ctx->progress_loaded)
        return LC_OK;
    path = progress_path(ctx, &a);
    if (!path && ctx->err_nomem) {
        rc = LC_ERR_NOMEM;
        goto done;
    }
    if (path)
        rc = read_progress(ctx, path, &text);
    if (rc == LC_OK && path)
        rc = parse_progress(ctx, path, lc_str_cstr(&text), text.len, 1, &arr,
                            &n, &cap);
    else if (rc != LC_OK && rc != LC_ERR_NOMEM)
        rc = lc_add_warning(ctx, "%s; your progress is not shown",
                            lc_last_error(ctx));
    if (rc == LC_OK) {
        free(ctx->progress);
        ctx->progress = arr;
        ctx->nprogress = n;
        ctx->capprogress = cap;
        ctx->progress_loaded = 1;
    }
done:
    lc_str_free(&text);
    lc_arena_free(&a);
    return rc;
}

static const lc_progress_entry *find_entry(const lc_progress_entry *arr,
                                           size_t n, int id)
{
    size_t lo = 0, hi = n;

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid].id == id)
            return &arr[mid];
        if (arr[mid].id < id)
            lo = mid + 1;
        else
            hi = mid;
    }
    return NULL;
}

int lc_progress(lc_ctx *ctx, int id, lc_progress_entry *out)
{
    const lc_progress_entry *e;
    int rc;

    if (!out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_progress: bad "
                            "arguments");
    empty_entry(id, out);
    rc = load_progress(ctx);
    if (rc != LC_OK)
        return rc;
    e = find_entry(ctx->progress, ctx->nprogress, id);
    if (e)
        *out = *e;
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Recording a submission                                              */
/* ------------------------------------------------------------------ */

void lc_record_info_free(lc_record_info *info)
{
    if (info)
        lc_str_free(&info->saved);
}

/* The line for an entry, with its newline. */
static void put_entry(lc_str *s, const lc_progress_entry *e)
{
    lc_str_printf(s, "%d\t%s\t%s\t", e->id, lc_status_name(e->status),
                  e->lang && *e->lang ? e->lang : "-");
    if (e->best_ms >= 0)
        lc_str_printf(s, "%ld", e->best_ms);
    else
        lc_str_putc(s, '-');
    if (e->last_ts > 0)
        lc_str_printf(s, "\t%lld\n", e->last_ts);
    else
        lc_str_puts(s, "\t-\n");
}

/* The id at the start of a line, or 0 when it has none. */
static int line_id(const char *p, size_t n)
{
    field f;
    const char *t = memchr(p, '\t', n);
    long long v;

    f.p = p;
    f.n = t ? (size_t)(t - p) : n;
    v = parse_num(f, MAX_ID);
    return v > 0 ? (int)v : 0;
}

/*
 * The new text of progress.tsv: `old` with the lines of entry e->id
 * replaced by one new line, which goes where the first of them was, or
 * else before the first line of a larger id, or else at the end.
 */
static void rewrite(lc_str *out, const char *old, size_t len,
                    const lc_progress_entry *e)
{
    const char *p = old, *end = old + len;
    int placed = 0;

    if (len == 0)
        lc_str_puts(out, header);
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *le = nl ? nl + 1 : end;
        int id = line_id(p, (size_t)(le - p));

        if (id == e->id) {
            if (!placed)
                put_entry(out, e);
            placed = 1;
        } else {
            if (id > e->id && !placed) {
                put_entry(out, e);
                placed = 1;
            }
            lc_str_append(out, p, (size_t)(le - p));
            if (!nl)
                lc_str_putc(out, '\n');
        }
        p = le;
    }
    if (!placed)
        put_entry(out, e);
}

/* Takes the lock on progress.tsv; *fd is -1 when there is no lock to be
 * had (a file system without locks), which is not an error. */
static void lock_progress(const char *dir, int *fd)
{
    lc_arena a = {NULL};
    char *path = lc_path_join(&a, dir, LOCK_FILE);
    struct flock fl;

    *fd = -1;
    if (path) {
        do
            *fd = open(path, O_RDWR | O_CREAT | O_CLOEXEC, 0666);
        while (*fd < 0 && errno == EINTR);
    }
    lc_arena_free(&a);
    if (*fd < 0)
        return;
    memset(&fl, 0, sizeof fl);
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    while (fcntl(*fd, F_SETLKW, &fl) != 0) {
        if (errno != EINTR) {
            close(*fd);
            *fd = -1;
            return;
        }
    }
}

/* The progress after a submission that got `v`. */
static void update_entry(lc_progress_entry *e, const lc_lang *lang,
                         lc_verdict v, long time_ms, int sanitize,
                         long long now)
{
    if (v == LC_ACCEPTED) {
        e->status = LC_SOLVED;
        e->lang = lang->name;
        if (!sanitize && (e->best_ms < 0 || time_ms < e->best_ms))
            e->best_ms = time_ms;
    } else {
        if (e->status != LC_SOLVED) {
            e->status = LC_ATTEMPTED;
            e->lang = lang->name;
        }
    }
    e->last_ts = now;
}

/* Copies the solution into the history; its path goes in `saved`. */
static int save_copy(lc_ctx *ctx, const char *dir, const lc_problem *p,
                     const lc_lang *lang, const char *src, lc_verdict v,
                     time_t now, lc_str *saved)
{
    lc_str text = LC_STR_INIT, path = LC_STR_INIT;
    char stamp[32];
    struct tm tm;
    size_t base;
    int rc, n, fd = -1;

    if (!gmtime_r(&now, &tm) ||
        strftime(stamp, sizeof stamp, "%Y%m%dT%H%M%SZ", &tm) == 0)
        snprintf(stamp, sizeof stamp, "%lld", (long long)now);
    rc = lc_read_file(ctx, src, MAX_SOURCE, &text);
    if (rc != LC_OK)
        goto done;
    lc_str_printf(&path, "%s/submissions/%04d", dir, p->id);
    if (path.failed) {
        rc = lc_oom(ctx);
        goto done;
    }
    rc = lc_mkdir_p(ctx, path.data, 0777);
    if (rc != LC_OK)
        goto done;
    lc_str_printf(&path, "/%s-%s", stamp, lc_verdict_id(v));
    base = path.len;
    /* Two submissions in the same second get -2, -3, ... */
    for (n = 1; n < 1000; n++) {
        path.len = base;
        if (n > 1)
            lc_str_printf(&path, "-%d", n);
        lc_str_printf(&path, ".%s", lang->ext);
        if (path.failed) {
            rc = lc_oom(ctx);
            goto done;
        }
        do
            fd = open(path.data, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC,
                      0666);
        while (fd < 0 && errno == EINTR);
        if (fd >= 0 || errno != EEXIST)
            break;
    }
    if (fd < 0) {
        rc = lc_set_errno(ctx, LC_ERR_IO, errno, "cannot create '%s'",
                          path.data);
        goto done;
    }
    {
        const char *d = lc_str_cstr(&text);
        size_t left = text.len;

        while (left > 0) {
            ssize_t w = write(fd, d, left);
            if (w < 0 && errno == EINTR)
                continue;
            if (w < 0) {
                rc = lc_set_errno(ctx, LC_ERR_IO, errno, "cannot write '%s'",
                                  path.data);
                close(fd);
                unlink(path.data);
                goto done;
            }
            d += w;
            left -= (size_t)w;
        }
    }
    if (close(fd) != 0) {
        rc = lc_set_errno(ctx, LC_ERR_IO, errno, "cannot write '%s'",
                          path.data);
        unlink(path.data);
        goto done;
    }
    lc_str_clear(saved);
    rc = lc_str_puts(saved, path.data) == LC_OK ? LC_OK : lc_oom(ctx);
done:
    lc_str_free(&text);
    lc_str_free(&path);
    return rc;
}

int lc_record(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
              const char *src, const lc_report *report, int sanitize,
              lc_record_info *info)
{
    lc_arena a = {NULL};
    lc_str text = LC_STR_INIT, out = LC_STR_INIT;
    lc_progress_entry *arr = NULL, e;
    const lc_progress_entry *old;
    size_t n = 0, cap = 0;
    const char *dir;
    char *path;
    time_t now = time(NULL);
    int rc, lock = -1;

    if (!info)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_record: bad arguments");
    memset(info, 0, sizeof *info);
    if (!p || !lang || !src || !report)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_record: bad arguments");
    if (report->cancelled || report->verdict == LC_VERDICT_NONE)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_record: a cancelled "
                            "run is not a submission");
    dir = lc_data_dir(ctx);
    if (!dir)
        return ctx->err_nomem ? LC_ERR_NOMEM : LC_ERR_NOTFOUND;
    rc = lc_mkdir_p(ctx, dir, 0777);
    if (rc != LC_OK)
        return rc;
    path = lc_path_join(&a, dir, PROGRESS_FILE);
    if (!path) {
        rc = lc_oom(ctx);
        goto done;
    }

    lock_progress(dir, &lock);
    rc = read_progress(ctx, path, &text);
    if (rc == LC_OK)
        rc = parse_progress(ctx, path, lc_str_cstr(&text), text.len, 0, &arr,
                            &n, &cap);
    if (rc != LC_OK)
        goto unlock;
    old = find_entry(arr, n, p->id);
    if (old)
        e = *old;
    else
        empty_entry(p->id, &e);
    info->before = e;
    update_entry(&e, lang, report->verdict, report->max_time_ms, sanitize,
                 (long long)now);
    rewrite(&out, lc_str_cstr(&text), text.len, &e);
    if (out.failed) {
        rc = lc_oom(ctx);
        goto unlock;
    }
    rc = lc_write_file_atomic(ctx, path, out.data, out.len);
    if (rc != LC_OK)
        goto unlock;
    info->after = e;
    info->recorded = 1;
    /* What was just written is now what lc_progress reports. */
    free(arr);
    arr = NULL;
    rc = parse_progress(ctx, path, out.data, out.len, 0, &arr, &n, &cap);
    if (rc == LC_OK) {
        free(ctx->progress);
        ctx->progress = arr;
        ctx->nprogress = n;
        ctx->capprogress = cap;
        ctx->progress_loaded = 1;
        arr = NULL;
    }
unlock:
    if (lock >= 0)
        close(lock);
    if (rc == LC_OK)
        rc = save_copy(ctx, dir, p, lang, src, report->verdict, now,
                       &info->saved);
done:
    free(arr);
    lc_str_free(&text);
    lc_str_free(&out);
    lc_arena_free(&a);
    return rc;
}

/* ------------------------------------------------------------------ */
/* Statistics                                                          */
/* ------------------------------------------------------------------ */

typedef struct stats_box {
    lc_arena arena;
    lc_stats_row *tags;       /* malloc'ed */
} stats_box;

static void count(lc_stats_row *r, lc_status st)
{
    r->total++;
    if (st == LC_SOLVED)
        r->solved++;
    else if (st == LC_ATTEMPTED)
        r->attempted++;
}

static int cmp_row(const void *a, const void *b)
{
    return strcmp(((const lc_stats_row *)a)->name,
                  ((const lc_stats_row *)b)->name);
}

int lc_stats(lc_ctx *ctx, lc_stats_summary *out)
{
    stats_box *box;
    size_t i, t, k, cap = 0;
    int rc;

    if (!out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_stats: bad arguments");
    memset(out, 0, sizeof *out);
    rc = load_progress(ctx);
    if (rc != LC_OK)
        return rc;
    box = calloc(1, sizeof *box);
    if (!box)
        return lc_oom(ctx);
    out->all.name = "all";
    for (i = 0; i < 4; i++)
        out->difficulty[i].name = lc_difficulty_name((lc_difficulty)i);
    for (i = 0; i < ctx->nproblems; i++) {
        const lc_problem *p = &ctx->problems[i];
        const lc_progress_entry *e = find_entry(ctx->progress,
                                                ctx->nprogress, p->id);
        lc_status st = e ? e->status : LC_TODO;

        count(&out->all, st);
        if ((unsigned)p->difficulty < 4)
            count(&out->difficulty[p->difficulty], st);
        for (t = 0; t < p->ntags; t++) {
            for (k = 0; k < out->ntags; k++)
                if (strcmp(box->tags[k].name, p->tags[t]) == 0)
                    break;
            if (k == out->ntags) {
                lc_stats_row *tmp = lc_grow(box->tags, &cap, k + 1,
                                            sizeof *box->tags);
                if (!tmp) {
                    free(box->tags);
                    free(box);
                    memset(out, 0, sizeof *out);
                    return lc_oom(ctx);
                }
                box->tags = tmp;
                memset(&box->tags[k], 0, sizeof box->tags[k]);
                box->tags[k].name = p->tags[t];
                out->ntags++;
            }
            count(&box->tags[k], st);
        }
    }
    if (out->ntags > 1)
        qsort(box->tags, out->ntags, sizeof *box->tags, cmp_row);
    out->tags = box->tags;
    out->priv_ = box;
    return LC_OK;
}

void lc_stats_free(lc_stats_summary *s)
{
    stats_box *box;

    if (!s || !s->priv_)
        return;
    box = s->priv_;
    free(box->tags);
    lc_arena_free(&box->arena);
    free(box);
    memset(s, 0, sizeof *s);
}
