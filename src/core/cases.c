#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/* cases.c: reads tests.txt (docs/problem-format.md, "tests.txt"). */

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "internal.h"

#define QUOTE_MAX 60 /* longest part of a line quoted in a message */

typedef struct span {
    const char *p;
    size_t n;
} span;

/* An lc_cases and the arena that holds everything it points to. */
typedef struct cases_box {
    lc_cases pub; /* first, so a pointer to it is a pointer to the box */
    lc_arena arena;
} cases_box;

static int is_blank_char(int c)
{
    return c == ' ' || c == '\t';
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

/* Like problem.c: the line at *pp without its newline or a '\r' before it. */
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

/* How much of p[0..n) to quote in a message: at most QUOTE_MAX bytes, cut at
 * a UTF-8 character boundary. */
static int quote_len(const char *p, size_t n)
{
    if (n > QUOTE_MAX) {
        n = QUOTE_MAX;
        while (n > 0 && ((unsigned char)p[n] & 0xC0) == 0x80)
            n--;
    }
    return (int)n;
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

static int parse_error(lc_ctx *ctx, const char *file, int line,
                       const char *fmt, ...) LC_PRINTF(4, 5);

/* Sets "<file>:<line>: <message>" and returns LC_ERR_PARSE (or NOMEM). */
static int parse_error(lc_ctx *ctx, const char *file, int line,
                       const char *fmt, ...)
{
    lc_str msg = LC_STR_INIT;
    va_list ap;

    lc_str_printf(&msg, "%s:%d: ", file, line);
    va_start(ap, fmt);
    lc_str_vprintf(&msg, fmt, ap);
    va_end(ap);
    if (msg.failed) {
        lc_str_free(&msg);
        return lc_oom(ctx);
    }
    lc_set_error(ctx, LC_ERR_PARSE, "%s", msg.data);
    lc_str_free(&msg);
    return error_status(ctx, LC_ERR_PARSE);
}

/* Copies the pending input lines into a new case at the end of *items. */
static int add_case(lc_arena *a, lc_case **items, size_t *n, size_t *cap,
                    const span *args, size_t nargs, span expected)
{
    lc_case *tmp = lc_grow(*items, cap, *n + 1, sizeof **items);
    char **copy;
    size_t i;

    if (!tmp)
        return LC_ERR_NOMEM;
    *items = tmp;
    copy = lc_arena_array(a, nargs, sizeof *copy);
    if (!copy)
        return LC_ERR_NOMEM;
    for (i = 0; i < nargs; i++) {
        copy[i] = lc_arena_strndup(a, args[i].p, args[i].n);
        if (!copy[i])
            return LC_ERR_NOMEM;
    }
    memset(&tmp[*n], 0, sizeof tmp[*n]);
    tmp[*n].args = (const char *const *)copy;
    tmp[*n].nargs = nargs;
    tmp[*n].expected = lc_arena_strndup(a, expected.p, expected.n);
    if (!tmp[*n].expected)
        return LC_ERR_NOMEM;
    (*n)++;
    return LC_OK;
}

int lc_parse_cases(lc_ctx *ctx, const char *text, size_t len,
                   const char *filename, lc_cases **out)
{
    cases_box *box;
    lc_case *items = NULL, *final;
    span *args = NULL;
    size_t nitems = 0, capitems = 0, nargs = 0, capargs = 0;
    const char *p, *end;
    const char *explanation = NULL;
    int lineno = 0, case_line = 0, example_line = 0, example = 0;
    int rc = LC_OK;

    if (!out)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_parse_cases: no output"));
    *out = NULL;
    if (!text && len)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_parse_cases: no text"));
    if (!filename)
        filename = "tests.txt";
    box = calloc(1, sizeof *box);
    if (!box)
        return lc_oom(ctx);

    p = text ? text : "";
    end = p + len;
    if (len >= 3 && memcmp(p, "\xEF\xBB\xBF", 3) == 0)
        p += 3;
    while (p < end) {
        span ln = trim(next_line(&p, end));

        if (lineno < INT_MAX)
            lineno++;
        if (memchr(ln.p, '\0', ln.n)) {
            /* It would silently cut the strings made from this line. */
            rc = parse_error(ctx, filename, lineno, "line contains a NUL byte");
            goto fail;
        }
        if (ln.n == 0) {
            if (nargs) {
                rc = parse_error(ctx, filename, lineno, "blank line inside a "
                                 "case (missing '=>'?)");
                goto fail;
            }
            continue;
        }
        if (ln.p[0] == '#')
            continue;

        if (ln.p[0] == '@') {
            size_t w = 1;
            while (w < ln.n && !is_blank_char((unsigned char)ln.p[w]))
                w++;
            if (w != 8 || memcmp(ln.p, "@example", 8) != 0) {
                rc = parse_error(ctx, filename, lineno, "unknown directive "
                                 "'%.*s%s'", quote_len(ln.p, w), ln.p,
                                 w > QUOTE_MAX ? "..." : "");
                goto fail;
            }
            if (nargs) {
                rc = parse_error(ctx, filename, lineno, "@example inside a "
                                 "case (missing '=>'?)");
                goto fail;
            }
            if (example) {
                rc = parse_error(ctx, filename, lineno, "two @example lines "
                                 "in a row");
                goto fail;
            }
            example = 1;
            example_line = lineno;
            explanation = NULL;
            if (w < ln.n) {
                span ex = trim((span){ln.p + w, ln.n - w});
                explanation = lc_arena_strndup(&box->arena, ex.p, ex.n);
                if (!explanation) {
                    rc = lc_oom(ctx);
                    goto fail;
                }
            }
            continue;
        }

        if (ln.n >= 2 && ln.p[0] == '=' && ln.p[1] == '>') {
            span expected = trim((span){ln.p + 2, ln.n - 2});
            if (!nargs) {
                rc = parse_error(ctx, filename, lineno, "'=>' with no input "
                                 "lines before it");
                goto fail;
            }
            if (expected.n == 0) {
                rc = parse_error(ctx, filename, lineno, "nothing after '=>'");
                goto fail;
            }
            if (add_case(&box->arena, &items, &nitems, &capitems, args, nargs,
                         expected) != LC_OK) {
                rc = lc_oom(ctx);
                goto fail;
            }
            items[nitems - 1].explanation = explanation;
            items[nitems - 1].example = example;
            items[nitems - 1].line = case_line;
            nargs = 0;
            example = 0;
            explanation = NULL;
            continue;
        }

        /* An input line: one literal, kept as text. */
        if (!nargs)
            case_line = lineno;
        {
            span *tmp = lc_grow(args, &capargs, nargs + 1, sizeof *args);
            if (!tmp) {
                rc = lc_oom(ctx);
                goto fail;
            }
            args = tmp;
            args[nargs++] = ln;
        }
    }
    if (nargs) {
        rc = parse_error(ctx, filename, case_line, "case has no '=>' line");
        goto fail;
    }
    if (example) {
        rc = parse_error(ctx, filename, example_line, "@example is not "
                         "followed by a case");
        goto fail;
    }

    final = lc_arena_array(&box->arena, nitems ? nitems : 1, sizeof *final);
    if (!final) {
        rc = lc_oom(ctx);
        goto fail;
    }
    if (nitems)
        memcpy(final, items, nitems * sizeof *final);
    box->pub.items = final;
    box->pub.count = nitems;
    box->pub.priv_ = &box->arena;
    free(items);
    free(args);
    *out = &box->pub;
    return LC_OK;
fail:
    free(items);
    free(args);
    lc_arena_free(&box->arena);
    free(box);
    return rc;
}

int lc_load_cases(lc_ctx *ctx, const lc_problem *p, lc_cases **out)
{
    lc_str path = LC_STR_INIT, text = LC_STR_INIT;
    struct stat st;
    int rc, found;

    if (!out)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_load_cases: no output"));
    *out = NULL;
    if (!p || !p->dir)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_load_cases: no problem"));
    if (lc_str_printf(&path, "%s/tests.txt", p->dir) != LC_OK) {
        lc_str_free(&path);
        return lc_oom(ctx);
    }
    /* A missing file is an empty list; check first so that case leaves the
     * last error alone. Only a regular file is read: opening a FIFO could
     * block forever, and a device could be endless. */
    found = stat(path.data, &st) == 0;
    if (!found && (errno == ENOENT || errno == ENOTDIR))
        rc = LC_ERR_NOTFOUND;
    else if (found && !S_ISREG(st.st_mode))
        rc = lc_set_error(ctx, LC_ERR_IO, "cannot read '%s': not a regular "
                          "file", path.data);
    else
        rc = lc_read_file(ctx, path.data, LC_MAX_TESTS_TXT, &text);
    if (rc == LC_ERR_NOTFOUND)
        rc = lc_parse_cases(ctx, "", 0, path.data, out);
    else if (rc == LC_OK)
        rc = lc_parse_cases(ctx, text.data, text.len, path.data, out);
    else
        rc = error_status(ctx, rc);
    lc_str_free(&text);
    lc_str_free(&path);
    return rc;
}

void lc_cases_free(lc_cases *cases)
{
    cases_box *box = (cases_box *)cases;

    if (!box)
        return;
    lc_arena_free(&box->arena);
    free(box);
}
