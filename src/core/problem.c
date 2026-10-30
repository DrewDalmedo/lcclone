#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * problem.c: finds the problem directory, loads every pack's problem.md
 * (docs/problem-format.md) and looks problems up.
 */

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "internal.h"

#define MAX_ID 99999
#define QUOTE_MAX 60      /* longest part of a value quoted in a warning */
#define SKIPPED (-1)      /* load_pack: the pack was left out with a warning */
#define NONE SIZE_MAX     /* pack.f: the field is absent */

typedef struct span {
    const char *p;
    size_t n;
} span;

/* One `key: value` line of the problem.md being read; points into its text. */
typedef struct hline {
    span key;
    span val;
    int line;
} hline;

enum {
    F_ID,
    F_SLUG,
    F_TITLE,
    F_DIFFICULTY, /* the fields before this one are required */
    F_TAGS,
    F_SIGNATURE,
    F_HIDDEN,
    F_TIME,
    F_MEMORY,
    NFIELDS
};

static const char *const field_names[NFIELDS] = {
    "id", "slug", "title", "difficulty", "tags", "signature", "hidden",
    "time_limit_ms", "memory_limit_mb"
};

/* Scratch state for one lc_load call, reused from pack to pack. */
typedef struct loader {
    lc_ctx *ctx;
    const char *root;    /* the problem directory (absolute) */
    lc_str buf;          /* text of the current problem.md */
    lc_str path;         /* "<root>/<name>/problem.md" */
    size_t dirlen;       /* length of the "<root>/<name>" part of path */
    lc_str msg;
    hline *lines;
    size_t nlines, caplines;
    const hline **order; /* lines sorted by key, to find duplicates */
    size_t caporder;
    span *tags;
    size_t ntags, captags;
    span *args;
    size_t nargs, capargs;
    lc_problem *probs;   /* accepted problems, in directory-name order */
    size_t nprobs, capprobs;
    size_t *ids;         /* hash sets over probs: index + 1, 0 when empty */
    size_t *slugs;
    size_t mask;
} loader;

/* What the checks learn about a pack before it is copied into the arena. */
typedef struct pack {
    size_t f[NFIELDS]; /* index into loader.lines, or NONE */
    int id;
    lc_difficulty difficulty;
    int time_ms;
    int mem_mb;
    const char *stmt;  /* into loader.buf */
    size_t stmt_len;
    int stmt_line;
} pack;

static span mkspan(const char *p, size_t n)
{
    span s;
    s.p = p;
    s.n = n;
    return s;
}

static int is_blank_char(int c)
{
    return c == ' ' || c == '\t';
}

static int is_digit(int c)
{
    return c >= '0' && c <= '9';
}

static int is_lower(int c)
{
    return c >= 'a' && c <= 'z';
}

static span trim(span s)
{
    while (s.n > 0 && is_blank_char((unsigned char)s.p[0])) {
        s.p++;
        s.n--;
    }
    while (s.n > 0 && is_blank_char((unsigned char)s.p[s.n - 1]))
        s.n--;
    return s;
}

static int span_eq(span s, const char *lit)
{
    size_t n = strlen(lit);
    return s.n == n && memcmp(s.p, lit, n) == 0;
}

static int next_lineno(int n)
{
    return n < INT_MAX ? n + 1 : n;
}

/*
 * Returns the line starting at *pp, without its newline or a '\r' before
 * the newline, and moves *pp to the next line.
 */
static span next_line(const char **pp, const char *end)
{
    const char *p = *pp;
    const char *nl = memchr(p, '\n', (size_t)(end - p));
    span s;

    s.p = p;
    if (nl) {
        s.n = (size_t)(nl - p);
        if (s.n > 0 && p[s.n - 1] == '\r')
            s.n--;
        *pp = nl + 1;
    } else {
        s.n = (size_t)(end - p);
        *pp = end;
    }
    return s;
}

/* Length of `s` to quote in a message: at most QUOTE_MAX bytes, cut at a
 * UTF-8 character boundary. Use with "'%.*s%s'" and QUOTE(). */
static int quote_len(span s)
{
    size_t n = s.n;

    if (n > QUOTE_MAX) {
        n = QUOTE_MAX;
        while (n > 0 && ((unsigned char)s.p[n] & 0xC0) == 0x80)
            n--;
    }
    return (int)n;
}

#define QUOTE(s) quote_len(s), (s).p, (s).n > QUOTE_MAX ? "..." : ""

/* A decimal number from 0 to max, or -1. */
static long parse_num(span s, long max)
{
    long v = 0;
    size_t i;

    if (s.n == 0)
        return -1;
    for (i = 0; i < s.n; i++) {
        int d = (unsigned char)s.p[i] - '0';
        if (d < 0 || d > 9 || v > (max - d) / 10)
            return -1;
        v = v * 10 + d;
    }
    return v;
}

/* Lowercase letters and digits in groups joined by single dashes. */
static int valid_slug(span s)
{
    int after_dash = 1; /* so a leading dash is rejected */
    size_t i;

    for (i = 0; i < s.n; i++) {
        int c = (unsigned char)s.p[i];
        if (c == '-') {
            if (after_dash)
                return 0;
            after_dash = 1;
        } else if (is_lower(c) || is_digit(c)) {
            after_dash = 0;
        } else {
            return 0;
        }
    }
    return s.n > 0 && !after_dash;
}

static int valid_tag(span s)
{
    size_t i;

    for (i = 0; i < s.n; i++) {
        int c = (unsigned char)s.p[i];
        if (!is_lower(c) && !is_digit(c) && c != '-')
            return 0;
    }
    return s.n > 0;
}

static int is_ident_char(int c)
{
    return is_lower(c) || (c >= 'A' && c <= 'Z') || is_digit(c) || c == '_';
}

/*
 * The status to return after a call that set the last error. lc_set_error
 * and lc_set_errno keep their status even when the message itself ran out
 * of memory, and that must reach the caller as LC_ERR_NOMEM.
 */
static int error_status(const lc_ctx *ctx, int rc)
{
    return ctx->err_nomem ? LC_ERR_NOMEM : rc;
}

static int pack_warn(loader *L, int line, const char *fmt, ...) LC_PRINTF(3, 4);

/*
 * Records "<path>:<line>: <message>" (or "<path>: <message>" when line is 0)
 * and returns SKIPPED, or LC_ERR_NOMEM.
 */
static int pack_warn(loader *L, int line, const char *fmt, ...)
{
    va_list ap;
    int rc;

    lc_str_clear(&L->msg);
    va_start(ap, fmt);
    lc_str_vprintf(&L->msg, fmt, ap);
    va_end(ap);
    if (L->msg.failed)
        return lc_oom(L->ctx);
    if (line > 0)
        rc = lc_add_warning(L->ctx, "%s:%d: %s", L->path.data, line,
                            L->msg.data);
    else
        rc = lc_add_warning(L->ctx, "%s: %s", L->path.data, L->msg.data);
    return rc == LC_OK ? SKIPPED : rc;
}

static int push_span(loader *L, span **arr, size_t *n, size_t *cap, span s)
{
    span *tmp = lc_grow(*arr, cap, *n + 1, sizeof **arr);

    if (!tmp)
        return lc_oom(L->ctx);
    *arr = tmp;
    (*arr)[(*n)++] = s;
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Header                                                              */
/* ------------------------------------------------------------------ */

/* The line that ends the header: "---", then only spaces and tabs. */
static int is_separator(span s)
{
    size_t i;

    if (s.n < 3 || memcmp(s.p, "---", 3) != 0)
        return 0;
    for (i = 3; i < s.n; i++)
        if (!is_blank_char((unsigned char)s.p[i]))
            return 0;
    return 1;
}

static int cmp_hline(const void *a, const void *b)
{
    const hline *x = *(const hline *const *)a;
    const hline *y = *(const hline *const *)b;
    size_t n = x->key.n < y->key.n ? x->key.n : y->key.n;
    int c = memcmp(x->key.p, y->key.p, n);

    if (c)
        return c;
    if (x->key.n != y->key.n)
        return x->key.n < y->key.n ? -1 : 1;
    return (x->line > y->line) - (x->line < y->line);
}

/* Reports the earliest line whose key already appeared. */
static int check_duplicate_keys(loader *L)
{
    const hline *dup = NULL, *first = NULL, *group;
    const hline **tmp;
    size_t i;

    if (L->nlines < 2)
        return LC_OK;
    tmp = lc_grow(L->order, &L->caporder, L->nlines, sizeof *L->order);
    if (!tmp)
        return lc_oom(L->ctx);
    L->order = tmp;
    for (i = 0; i < L->nlines; i++)
        L->order[i] = &L->lines[i];
    qsort(L->order, L->nlines, sizeof *L->order, cmp_hline);
    group = L->order[0];
    for (i = 1; i < L->nlines; i++) {
        const hline *h = L->order[i];
        if (h->key.n != group->key.n ||
            memcmp(h->key.p, group->key.p, h->key.n) != 0) {
            group = h;
            continue;
        }
        if (!dup || h->line < dup->line) {
            dup = h;
            first = group;
        }
    }
    if (dup)
        return pack_warn(L, dup->line, "duplicate key '%.*s%s' (first on "
                         "line %d)", QUOTE(dup->key), first->line);
    return LC_OK;
}

/* Splits problem.md into header lines and statement. */
static int parse_file(loader *L, pack *pk)
{
    const char *data = L->buf.data ? L->buf.data : "";
    const char *end = data + L->buf.len;
    const char *p = data, *hdr, *hdr_end = NULL;
    int lineno = 0, rc;
    size_t i, k;

    /* A NUL byte would silently cut the strings made from this file. */
    {
        const char *nul = memchr(data, '\0', L->buf.len);
        if (nul) {
            const char *q;
            for (q = data; q < nul; q++)
                if (*q == '\n')
                    lineno = next_lineno(lineno);
            return pack_warn(L, next_lineno(lineno), "contains a NUL byte");
        }
    }
    if (L->buf.len >= 3 && memcmp(p, "\xEF\xBB\xBF", 3) == 0)
        p += 3;
    hdr = p;

    /* Find the end of the header first, so that a file without a '---'
     * line gets that message rather than one about its statement. */
    while (p < end) {
        const char *start = p;
        span ln = next_line(&p, end);
        lineno = next_lineno(lineno);
        if (is_separator(ln)) {
            hdr_end = start;
            break;
        }
    }
    if (!hdr_end)
        return pack_warn(L, 0, "missing '---' line after the header");
    pk->stmt = p;
    pk->stmt_len = (size_t)(end - p);
    pk->stmt_line = next_lineno(lineno);

    L->nlines = 0;
    lineno = 0;
    p = hdr;
    while (p < hdr_end) {
        span ln = next_line(&p, hdr_end);
        hline *tmp;
        size_t n = 0;

        lineno = next_lineno(lineno);
        if (trim(ln).n == 0 || ln.p[0] == '#')
            continue;
        while (n < ln.n && (is_lower((unsigned char)ln.p[n]) ||
                            is_digit((unsigned char)ln.p[n]) || ln.p[n] == '_'))
            n++;
        if (n == 0 || n == ln.n || ln.p[n] != ':' ||
            is_digit((unsigned char)ln.p[0])) {
            const char *colon = memchr(ln.p, ':', ln.n);
            span key;
            if (!colon)
                return pack_warn(L, lineno, "expected 'key: value'");
            key = mkspan(ln.p, (size_t)(colon - ln.p));
            return pack_warn(L, lineno, "invalid key '%.*s%s'", QUOTE(key));
        }
        tmp = lc_grow(L->lines, &L->caplines, L->nlines + 1, sizeof *L->lines);
        if (!tmp)
            return lc_oom(L->ctx);
        L->lines = tmp;
        L->lines[L->nlines].key = mkspan(ln.p, n);
        L->lines[L->nlines].val = trim(mkspan(ln.p + n + 1, ln.n - n - 1));
        L->lines[L->nlines].line = lineno;
        L->nlines++;
    }

    rc = check_duplicate_keys(L);
    if (rc != LC_OK)
        return rc;

    for (k = 0; k < NFIELDS; k++)
        pk->f[k] = NONE;
    for (i = 0; i < L->nlines; i++) {
        for (k = 0; k < NFIELDS; k++) {
            if (span_eq(L->lines[i].key, field_names[k])) {
                pk->f[k] = i;
                break;
            }
        }
    }
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Field checks                                                        */
/* ------------------------------------------------------------------ */

static int parse_tags(loader *L, const hline *h)
{
    const char *p = h->val.p, *end = h->val.p + h->val.n;

    if (h->val.n == 0)
        return LC_OK;
    for (;;) {
        const char *comma = memchr(p, ',', (size_t)(end - p));
        span tag = trim(mkspan(p, (size_t)((comma ? comma : end) - p)));
        int rc;

        if (tag.n == 0)
            return pack_warn(L, h->line, "empty tag in '%.*s%s'",
                             QUOTE(h->val));
        if (!valid_tag(tag))
            return pack_warn(L, h->line, "invalid tag '%.*s%s' (use lowercase "
                             "letters, digits and dashes)", QUOTE(tag));
        rc = push_span(L, &L->tags, &L->ntags, &L->captags, tag);
        if (rc != LC_OK)
            return rc;
        if (!comma)
            return LC_OK;
        p = comma + 1;
    }
}

/*
 * Appends the parameter names in `list` to L->args. Parameters are split
 * on commas outside <>, [] and (); each one's name is the identifier at
 * its end, and something (its type) must come before it.
 */
static int parse_params(loader *L, span list, const char *what, int line)
{
    size_t i, start = 0, count = 0;
    int depth = 0;

    if (trim(list).n == 0)
        return LC_OK;
    for (i = 0; i <= list.n; i++) {
        span param, name;
        size_t j;
        int rc;

        if (i < list.n) {
            char c = list.p[i];
            if (c == '<' || c == '[' || c == '(')
                depth++;
            else if ((c == '>' || c == ']' || c == ')') && depth > 0)
                depth--;
            if (c != ',' || depth > 0)
                continue;
        }
        count++;
        param = trim(mkspan(list.p + start, i - start));
        start = i + 1;
        if (param.n == 0)
            return pack_warn(L, line, "%s: parameter %lu is empty", what,
                             (unsigned long)count);
        j = param.n;
        while (j > 0 && is_ident_char((unsigned char)param.p[j - 1]))
            j--;
        while (j < param.n && is_digit((unsigned char)param.p[j]))
            j++;
        name = mkspan(param.p + j, param.n - j);
        if (name.n == 0 || trim(mkspan(param.p, j)).n == 0)
            return pack_warn(L, line, "%s: parameter '%.*s%s' has no name",
                             what, QUOTE(param));
        rc = push_span(L, &L->args, &L->nargs, &L->capargs, name);
        if (rc != LC_OK)
            return rc;
    }
    return LC_OK;
}

static int parse_signature(loader *L, const hline *h)
{
    const char *open = memchr(h->val.p, '(', h->val.n);
    const char *close = NULL;
    size_t i;

    for (i = h->val.n; i > 0; i--) {
        if (h->val.p[i - 1] == ')') {
            close = h->val.p + i - 1;
            break;
        }
    }
    if (!open || !close || close < open)
        return pack_warn(L, h->line, "signature has no parentheses");
    return parse_params(L, mkspan(open + 1, (size_t)(close - open - 1)),
                        "signature", h->line);
}

static int parse_limit(loader *L, const pack *pk, int field, int def, int *out)
{
    const hline *h;
    long v;

    *out = def;
    if (pk->f[field] == NONE)
        return LC_OK;
    h = &L->lines[pk->f[field]];
    v = parse_num(h->val, INT_MAX);
    if (v < 1)
        return pack_warn(L, h->line, "invalid %s '%.*s%s' (expected a positive "
                         "integer)", field_names[field], QUOTE(h->val));
    *out = (int)v;
    return LC_OK;
}

static int check_fields(loader *L, const char *name, pack *pk)
{
    const hline *h;
    span slug;
    char prefix[16];
    size_t plen;
    long v;
    int k, rc;

    for (k = F_ID; k <= F_DIFFICULTY; k++)
        if (pk->f[k] == NONE)
            return pack_warn(L, 0, "missing required field '%s'",
                             field_names[k]);

    h = &L->lines[pk->f[F_ID]];
    v = parse_num(h->val, MAX_ID);
    if (v < 1)
        return pack_warn(L, h->line, "invalid id '%.*s%s' (expected an integer "
                         "from 1 to %d)", QUOTE(h->val), MAX_ID);
    pk->id = (int)v;

    h = &L->lines[pk->f[F_SLUG]];
    slug = h->val;
    if (!valid_slug(slug))
        return pack_warn(L, h->line, "invalid slug '%.*s%s' (expected "
                         "lowercase letters and digits joined by single "
                         "dashes)", QUOTE(slug));

    h = &L->lines[pk->f[F_TITLE]];
    if (h->val.n == 0)
        return pack_warn(L, h->line, "empty title");

    h = &L->lines[pk->f[F_DIFFICULTY]];
    if (span_eq(h->val, "easy"))
        pk->difficulty = LC_EASY;
    else if (span_eq(h->val, "medium"))
        pk->difficulty = LC_MEDIUM;
    else if (span_eq(h->val, "hard"))
        pk->difficulty = LC_HARD;
    else
        return pack_warn(L, h->line, "unknown difficulty '%.*s%s'",
                         QUOTE(h->val));

    L->ntags = 0;
    if (pk->f[F_TAGS] != NONE) {
        rc = parse_tags(L, &L->lines[pk->f[F_TAGS]]);
        if (rc != LC_OK)
            return rc;
    }

    L->nargs = 0;
    if (pk->f[F_SIGNATURE] != NONE) {
        rc = parse_signature(L, &L->lines[pk->f[F_SIGNATURE]]);
        if (rc != LC_OK)
            return rc;
    }
    if (pk->f[F_HIDDEN] != NONE) {
        h = &L->lines[pk->f[F_HIDDEN]];
        rc = parse_params(L, h->val, "hidden", h->line);
        if (rc != LC_OK)
            return rc;
    }
    if (pk->f[F_SIGNATURE] == NONE)
        L->nargs = 0; /* lc.h: no names without a signature */

    rc = parse_limit(L, pk, F_TIME, 1000, &pk->time_ms);
    if (rc != LC_OK)
        return rc;
    rc = parse_limit(L, pk, F_MEMORY, 256, &pk->mem_mb);
    if (rc != LC_OK)
        return rc;

    /* The directory must be named printf("%04d-%s", id, slug). */
    plen = (size_t)snprintf(prefix, sizeof prefix, "%04d-", pk->id);
    if (strlen(name) != plen + slug.n || memcmp(name, prefix, plen) != 0 ||
        memcmp(name + plen, slug.p, slug.n) != 0)
        return pack_warn(L, 0, "directory name '%s' does not match id and slug "
                         "(expected '%s%.*s%s')", name, prefix, QUOTE(slug));
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Accepting a pack                                                    */
/* ------------------------------------------------------------------ */

static size_t hash_span(span s)
{
    size_t h = 2166136261u, i;

    for (i = 0; i < s.n; i++)
        h = (h ^ (unsigned char)s.p[i]) * 16777619u;
    return h;
}

/* The slot holding the accepted problem with this id, or an empty slot. */
static size_t *id_slot(loader *L, int id)
{
    size_t i = ((size_t)id * 2654435761u) & L->mask;

    while (L->ids[i] && L->probs[L->ids[i] - 1].id != id)
        i = (i + 1) & L->mask;
    return &L->ids[i];
}

static size_t *slug_slot(loader *L, span slug)
{
    size_t i = hash_span(slug) & L->mask;

    while (L->slugs[i] && !span_eq(slug, L->probs[L->slugs[i] - 1].slug))
        i = (i + 1) & L->mask;
    return &L->slugs[i];
}

static const char *base_name(const char *path)
{
    const char *s = strrchr(path, '/');
    return s ? s + 1 : path;
}

static const char *const *copy_spans(lc_arena *a, const span *s, size_t n)
{
    char **out = lc_arena_array(a, n ? n : 1, sizeof *out);
    size_t i;

    if (!out)
        return NULL;
    for (i = 0; i < n; i++) {
        out[i] = lc_arena_strndup(a, s[i].p, s[i].n);
        if (!out[i])
            return NULL;
    }
    return (const char *const *)out;
}

/* Copies text into the arena, dropping each '\r' that comes before a '\n'. */
static char *copy_text(lc_arena *a, const char *s, size_t n, size_t *outlen)
{
    char *d;
    size_t i, j = 0;

    if (!memchr(s, '\r', n)) {
        *outlen = n;
        return lc_arena_strndup(a, s, n);
    }
    d = lc_arena_alloc(a, n + 1);
    if (!d)
        return NULL;
    for (i = 0; i < n; i++) {
        if (s[i] == '\r' && i + 1 < n && s[i + 1] == '\n')
            continue;
        d[j++] = s[i];
    }
    d[j] = '\0';
    *outlen = j;
    return d;
}

/* Copies a checked pack into the context's arena and appends it to probs. */
static int add_problem(loader *L, const pack *pk)
{
    lc_arena *a = &L->ctx->arena;
    lc_header_field *fields;
    lc_problem *p, *tmp;
    char *stmt;
    size_t i;

    tmp = lc_grow(L->probs, &L->capprobs, L->nprobs + 1, sizeof *L->probs);
    if (!tmp)
        return lc_oom(L->ctx);
    L->probs = tmp;
    p = &L->probs[L->nprobs];
    memset(p, 0, sizeof *p);

    fields = lc_arena_array(a, L->nlines, sizeof *fields);
    if (!fields)
        return lc_oom(L->ctx);
    for (i = 0; i < L->nlines; i++) {
        const hline *h = &L->lines[i];
        fields[i].key = lc_arena_strndup(a, h->key.p, h->key.n);
        fields[i].value = lc_arena_strndup(a, h->val.p, h->val.n);
        fields[i].line = h->line;
        if (!fields[i].key || !fields[i].value)
            return lc_oom(L->ctx);
    }
    p->fields = fields;
    p->nfields = L->nlines;
    p->id = pk->id;
    p->slug = fields[pk->f[F_SLUG]].value;
    p->title = fields[pk->f[F_TITLE]].value;
    p->difficulty = pk->difficulty;
    if (pk->f[F_SIGNATURE] != NONE)
        p->signature = fields[pk->f[F_SIGNATURE]].value;
    p->time_limit_ms = pk->time_ms;
    p->memory_limit_mb = pk->mem_mb;
    p->tags = copy_spans(a, L->tags, L->ntags);
    p->ntags = L->ntags;
    p->arg_names = copy_spans(a, L->args, L->nargs);
    p->narg_names = L->nargs;
    p->dir = lc_arena_strndup(a, L->path.data, L->dirlen);
    stmt = copy_text(a, pk->stmt, pk->stmt_len, &p->statement_len);
    p->statement = stmt;
    p->statement_line = pk->stmt_line;
    if (!p->tags || !p->arg_names || !p->dir || !stmt)
        return lc_oom(L->ctx);
    L->nprobs++;
    return LC_OK;
}

/*
 * Loads the pack whose directory is L->path (`name` inside the problem
 * directory). Returns LC_OK when it was added, SKIPPED after recording a
 * warning, or LC_ERR_NOMEM.
 */
static int load_pack(loader *L, const char *name)
{
    pack pk;
    const hline *hid, *hslug;
    size_t *ids, *slugs;
    struct stat st;
    int rc;

    if (lc_str_puts(&L->path, "/problem.md") != LC_OK)
        return lc_oom(L->ctx);
    /* Opening a FIFO could block forever, and a device could be endless. */
    if (stat(L->path.data, &st) == 0 && !S_ISREG(st.st_mode))
        return pack_warn(L, 0, "not a regular file");
    lc_str_clear(&L->buf);
    rc = lc_read_file(L->ctx, L->path.data, &L->buf);
    if (rc != LC_OK)
        rc = error_status(L->ctx, rc);
    if (rc == LC_ERR_NOMEM)
        return rc;
    if (rc == LC_ERR_NOTFOUND)
        return pack_warn(L, 0, "file not found");
    if (rc != LC_OK)
        return pack_warn(L, 0, "%s", lc_last_error(L->ctx));

    memset(&pk, 0, sizeof pk);
    rc = parse_file(L, &pk);
    if (rc != LC_OK)
        return rc;
    rc = check_fields(L, name, &pk);
    if (rc != LC_OK)
        return rc;

    /* Directories are visited in name order, so the first one wins. */
    hid = &L->lines[pk.f[F_ID]];
    hslug = &L->lines[pk.f[F_SLUG]];
    ids = id_slot(L, pk.id);
    if (*ids)
        return pack_warn(L, hid->line, "duplicate id %d (already used by %s)",
                         pk.id, base_name(L->probs[*ids - 1].dir));
    slugs = slug_slot(L, hslug->val);
    if (*slugs)
        return pack_warn(L, hslug->line, "duplicate slug '%.*s%s' (already "
                         "used by %s)", QUOTE(hslug->val),
                         base_name(L->probs[*slugs - 1].dir));
    rc = add_problem(L, &pk);
    if (rc != LC_OK)
        return rc;
    *ids = *slugs = L->nprobs;
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Finding the problem directory                                       */
/* ------------------------------------------------------------------ */

/* A directory the user named: it must exist. */
static int use_given_dir(lc_ctx *ctx, const char *dir, const char *label,
                         char **out)
{
    struct stat st;
    char *real;
    int e, rc;

    if (stat(dir, &st) != 0) {
        e = errno;
        rc = e == ENOENT || e == ENOTDIR ? LC_ERR_NOTFOUND : LC_ERR_IO;
        return error_status(ctx, lc_set_errno(ctx, rc, e, "cannot use %s '%s'",
                                              label, dir));
    }
    if (!S_ISDIR(st.st_mode))
        return error_status(ctx, lc_set_errno(ctx, LC_ERR_NOTFOUND, ENOTDIR,
                                              "cannot use %s '%s'", label,
                                              dir));
    real = lc_realpath(&ctx->arena, dir);
    if (!real) {
        e = errno;
        if (e == ENOMEM)
            return lc_oom(ctx);
        return error_status(ctx, lc_set_errno(ctx, LC_ERR_IO, e, "cannot use "
                                              "%s '%s'", label, dir));
    }
    *out = real;
    return LC_OK;
}

static int use_found_dir(lc_ctx *ctx, const char *dir, char **out)
{
    char *real = lc_realpath(&ctx->arena, dir);
    int e;

    if (!real) {
        e = errno;
        if (e == ENOMEM)
            return lc_oom(ctx);
        return error_status(ctx, lc_set_errno(ctx, LC_ERR_IO, e, "cannot use "
                                              "problem directory '%s'", dir));
    }
    *out = real;
    return LC_OK;
}

/* Picks the problem directory (docs/problem-format.md, "Where packs live"). */
static int find_problems_dir(lc_ctx *ctx, char **out)
{
    static const char datadir[] = LC_DATADIR "/problems";
    lc_arena tmp = {0};
    const char *env, *xdg_env;
    char *xdg, *path = NULL;
    int rc;

    if (ctx->opt_problems_dir)
        return use_given_dir(ctx, ctx->opt_problems_dir, "problem directory",
                             out);
    env = lc_getenv("LC_PROBLEMS");
    if (env)
        return use_given_dir(ctx, env, "LC_PROBLEMS directory", out);

    xdg = lc_xdg_dir(&tmp, "XDG_DATA_HOME", ".local/share");
    if (xdg) {
        path = lc_path_join(&tmp, xdg, "lcclone/problems");
        if (!path) {
            lc_arena_free(&tmp);
            return lc_oom(ctx);
        }
    } else {
        /* NULL means out of memory when a base directory was usable. */
        xdg_env = lc_getenv("XDG_DATA_HOME");
        if ((xdg_env && xdg_env[0] == '/') || lc_getenv("HOME")) {
            lc_arena_free(&tmp);
            return lc_oom(ctx);
        }
    }
    if (path && lc_is_dir(path))
        rc = use_found_dir(ctx, path, out);
    else if (lc_is_dir(datadir))
        rc = use_found_dir(ctx, datadir, out);
    else if (path)
        rc = error_status(ctx, lc_set_error(ctx, LC_ERR_NOTFOUND, "no problem "
                          "directory found (tried '%s' and '%s'); set "
                          "LC_PROBLEMS to the directory that holds the "
                          "problem packs", path, datadir));
    else
        rc = error_status(ctx, lc_set_error(ctx, LC_ERR_NOTFOUND, "no problem "
                          "directory found (tried '%s'); set LC_PROBLEMS to "
                          "the directory that holds the problem packs",
                          datadir));
    lc_arena_free(&tmp);
    return rc;
}

/* ------------------------------------------------------------------ */
/* Public functions                                                    */
/* ------------------------------------------------------------------ */

static int cmp_problem_id(const void *a, const void *b)
{
    const lc_problem *x = a, *y = b;
    return (x->id > y->id) - (x->id < y->id);
}

int lc_load(lc_ctx *ctx)
{
    loader L;
    lc_arena scratch = {0};
    lc_problem *probs;
    char **names = NULL;
    size_t nnames = 0, ncand = 0, cap = 16, i;
    char *root = NULL;
    int rc;

    if (ctx->loaded)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID, "problems "
                                              "are already loaded"));
    rc = find_problems_dir(ctx, &root);
    if (rc != LC_OK)
        return rc;
    rc = lc_list_dir(ctx, root, &scratch, &names, &nnames);
    if (rc != LC_OK) {
        lc_arena_free(&scratch);
        return error_status(ctx, rc);
    }

    memset(&L, 0, sizeof L);
    L.ctx = ctx;
    L.root = strcmp(root, "/") == 0 ? "" : root;
    for (i = 0; i < nnames; i++)
        if (is_digit((unsigned char)names[i][0]))
            ncand++;
    while (cap < ncand * 2 && cap <= SIZE_MAX / 4)
        cap *= 2;
    L.ids = calloc(cap, sizeof *L.ids);
    L.slugs = calloc(cap, sizeof *L.slugs);
    if (!L.ids || !L.slugs || cap < ncand * 2) {
        rc = lc_oom(ctx);
        goto done;
    }
    L.mask = cap - 1;

    for (i = 0; i < nnames; i++) {
        if (!is_digit((unsigned char)names[i][0]))
            continue;
        lc_str_clear(&L.path);
        if (lc_str_printf(&L.path, "%s/%s", L.root, names[i]) != LC_OK) {
            rc = lc_oom(ctx);
            goto done;
        }
        L.dirlen = L.path.len;
        if (!lc_is_dir(L.path.data))
            continue;
        rc = load_pack(&L, names[i]);
        if (rc == SKIPPED)
            rc = LC_OK;
        else if (rc != LC_OK)
            goto done;
    }

    probs = lc_arena_array(&ctx->arena, L.nprobs ? L.nprobs : 1, sizeof *probs);
    if (!probs) {
        rc = lc_oom(ctx);
        goto done;
    }
    if (L.nprobs) {
        memcpy(probs, L.probs, L.nprobs * sizeof *probs);
        qsort(probs, L.nprobs, sizeof *probs, cmp_problem_id);
    }
    ctx->problems = probs;
    ctx->nproblems = L.nprobs;
    ctx->problems_dir = root;
    ctx->loaded = 1;
done:
    lc_str_free(&L.buf);
    lc_str_free(&L.path);
    lc_str_free(&L.msg);
    free(L.lines);
    free(L.order);
    free(L.tags);
    free(L.args);
    free(L.probs);
    free(L.ids);
    free(L.slugs);
    lc_arena_free(&scratch);
    return rc;
}

size_t lc_problem_count(const lc_ctx *ctx)
{
    return ctx->nproblems;
}

const lc_problem *lc_problem_at(const lc_ctx *ctx, size_t i)
{
    return i < ctx->nproblems ? &ctx->problems[i] : NULL;
}

static const lc_problem *find_id(const lc_ctx *ctx, long id)
{
    size_t lo = 0, hi = ctx->nproblems;

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        int pid = ctx->problems[mid].id;
        if (pid == id)
            return &ctx->problems[mid];
        if (pid < id)
            lo = mid + 1;
        else
            hi = mid;
    }
    return NULL;
}

const lc_problem *lc_problem_find(lc_ctx *ctx, const char *key)
{
    const lc_problem *p;
    size_t i, ndig = 0;
    long id;

    if (!ctx->loaded) {
        lc_set_error(ctx, LC_ERR_INVALID, "problems are not loaded");
        return NULL;
    }
    if (!key || !*key) {
        lc_set_error(ctx, LC_ERR_INVALID, "no problem given");
        return NULL;
    }
    for (i = 0; i < ctx->nproblems; i++)
        if (strcmp(ctx->problems[i].slug, key) == 0)
            return &ctx->problems[i];

    while (is_digit((unsigned char)key[ndig]))
        ndig++;
    if (ndig > 0 && (key[ndig] == '\0' || key[ndig] == '-')) {
        /* A number too big to be an id matches nothing. */
        id = parse_num(mkspan(key, ndig), MAX_ID);
        p = id > 0 ? find_id(ctx, id) : NULL;
        if (p && (key[ndig] == '\0' || strcmp(p->slug, key + ndig + 1) == 0))
            return p;
    }
    lc_set_error(ctx, LC_ERR_NOTFOUND, "no problem matches '%s'", key);
    return NULL;
}

const char *lc_problem_field(const lc_problem *p, const char *key)
{
    size_t i;

    if (!p || !key)
        return NULL;
    for (i = 0; i < p->nfields; i++)
        if (strcmp(p->fields[i].key, key) == 0)
            return p->fields[i].value;
    return NULL;
}

int lc_problem_has_tag(const lc_problem *p, const char *tag)
{
    size_t i;

    if (!p || !tag)
        return 0;
    for (i = 0; i < p->ntags; i++)
        if (strcmp(p->tags[i], tag) == 0)
            return 1;
    return 0;
}
