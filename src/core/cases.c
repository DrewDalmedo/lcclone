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
#include <stdio.h>
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

/* What is known about a case before it ends. */
typedef struct pending {
    int example;
    int example_line;
    const char *explanation;
    const char *gen;          /* @gen arguments, in the arena */
    int gen_line;
    int line;                 /* first input line */
    int last_line;            /* last input line */
} pending;

/* Copies the pending input lines into a new case at the end of *items.
 * `expected` is NULL for a case without one (yours, or --regen's). */
static int end_case(lc_arena *a, lc_case **items, size_t *n, size_t *cap,
                    const span *args, size_t nargs, const span *expected,
                    int expected_line, const pending *pd, const char *file)
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
    tmp[*n].example = pd->example;
    tmp[*n].explanation = pd->explanation;
    tmp[*n].gen = pd->gen;
    tmp[*n].line = pd->line ? pd->line : pd->gen_line;
    tmp[*n].last_line = pd->last_line;
    tmp[*n].expected_line = expected_line;
    tmp[*n].file = file;
    if (expected) {
        tmp[*n].expected = lc_arena_strndup(a, expected->p, expected->n);
        if (!tmp[*n].expected)
            return LC_ERR_NOMEM;
    }
    (*n)++;
    return LC_OK;
}

/*
 * Parses "@gen ARGS": a seed (a decimal number that fits in 64 bits),
 * then optionally "small". Returns the arguments in the canonical form
 * "SEED" or "SEED small", in `a`; NULL with *bad set when they are
 * malformed, or NULL alone when out of memory.
 */
static const char *parse_gen(lc_arena *a, span args, int *bad)
{
    const char *p = args.p, *end = args.p + args.n, *s;
    unsigned long long seed = 0;
    int small = 0;

    *bad = 0;
    for (s = p; p < end && *p >= '0' && *p <= '9'; p++) {
        unsigned d = (unsigned)(*p - '0');
        if (seed > (18446744073709551615ull - d) / 10) {
            *bad = 1;
            return NULL;
        }
        seed = seed * 10 + d;
    }
    if (p == s || (p - s > 1 && *s == '0')) {
        *bad = 1;
        return NULL;
    }
    while (p < end && is_blank_char((unsigned char)*p))
        p++;
    if (p < end) {
        if ((size_t)(end - p) != 5 || memcmp(p, "small", 5) != 0) {
            *bad = 1;
            return NULL;
        }
        small = 1;
    }
    return lc_arena_printf(a, "%llu%s", seed, small ? " small" : "");
}

/* What parse_cases reads. */
enum {
    MODE_TESTS,  /* a pack's tests.txt */
    MODE_USER,   /* your cases: "=> expected" may be left out */
    MODE_REGEN   /* tests.txt for --regen: "=> expected" may be left out,
                    and a case after @gen may have no inputs yet */
};

/*
 * Parses tests.txt or a file of cases (see the modes above). In MODE_USER
 * and MODE_REGEN a case without "=>" ends at a blank line, a directive or
 * the end of the file.
 */
static int parse_cases(lc_ctx *ctx, const char *text, size_t len,
                       const char *filename, int mode, lc_cases **out)
{
    cases_box *box;
    lc_case *items = NULL, *final;
    span *args = NULL;
    size_t nitems = 0, capitems = 0, nargs = 0, capargs = 0;
    const char *p, *end;
    pending pd;
    int lineno = 0;
    int noinput = 0; /* the open case is "()": it has no inputs */
    int rc = LC_OK;

#define OPEN (nargs || noinput)
#define FLUSH()                                                            \
    do {                                                                   \
        if (end_case(&box->arena, &items, &nitems, &capitems, args, nargs, \
                     NULL, 0, &pd, filename) != LC_OK) {                   \
            rc = lc_oom(ctx);                                              \
            goto fail;                                                     \
        }                                                                  \
        nargs = 0;                                                         \
        noinput = 0;                                                       \
        memset(&pd, 0, sizeof pd);                                         \
    } while (0)

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
    filename = lc_arena_strdup(&box->arena, filename);
    if (!filename) {
        free(box);
        return lc_oom(ctx);
    }
    memset(&pd, 0, sizeof pd);

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
            /* An @gen must be followed right away by its case, so that a
             * bare "@gen 7" line is a case for --regen to make. */
            if ((OPEN && mode != MODE_TESTS) || (pd.gen && mode == MODE_REGEN))
                FLUSH();
            else if (OPEN) {
                rc = parse_error(ctx, filename, lineno, "blank line inside a "
                                 "case (missing '=>'?)");
                goto fail;
            } else if (pd.gen) {
                rc = parse_error(ctx, filename, pd.gen_line, "@gen is not "
                                 "followed by a case (run 'lc check --regen' "
                                 "to make it)");
                goto fail;
            }
            continue;
        }
        if (ln.p[0] == '#')
            continue;

        if (ln.p[0] == '@') {
            size_t w = 1;
            int is_gen;

            while (w < ln.n && !is_blank_char((unsigned char)ln.p[w]))
                w++;
            is_gen = w == 4 && memcmp(ln.p, "@gen", 4) == 0;
            if (!is_gen && (w != 8 || memcmp(ln.p, "@example", 8) != 0)) {
                rc = parse_error(ctx, filename, lineno, "unknown directive "
                                 "'%.*s%s'", quote_len(ln.p, w), ln.p,
                                 w > QUOTE_MAX ? "..." : "");
                goto fail;
            }
            if (mode == MODE_USER) {
                rc = parse_error(ctx, filename, lineno, "%.*s is only for a "
                                 "problem's tests.txt", (int)w, ln.p);
                goto fail;
            }
            if (mode == MODE_REGEN && (OPEN || pd.gen))
                FLUSH();
            if (OPEN) {
                rc = parse_error(ctx, filename, lineno, "%.*s inside a case "
                                 "(missing '=>'?)", (int)w, ln.p);
                goto fail;
            }
            if ((is_gen && pd.gen) || (!is_gen && pd.example)) {
                rc = parse_error(ctx, filename, lineno, "two %.*s lines in a "
                                 "row", (int)w, ln.p);
                goto fail;
            }
            if (pd.gen || pd.example) {
                rc = parse_error(ctx, filename, lineno, "a case can't be "
                                 "both @example and @gen");
                goto fail;
            }
            if (is_gen) {
                int bad;

                pd.gen = parse_gen(&box->arena, trim((span){ln.p + w,
                                                            ln.n - w}),
                                   &bad);
                if (bad) {
                    rc = parse_error(ctx, filename, lineno, "expected '@gen "
                                     "SEED' or '@gen SEED small', where "
                                     "SEED is a number");
                    goto fail;
                }
                if (!pd.gen) {
                    rc = lc_oom(ctx);
                    goto fail;
                }
                pd.gen_line = lineno;
                continue;
            }
            pd.example = 1;
            pd.example_line = lineno;
            if (w < ln.n) {
                span ex = trim((span){ln.p + w, ln.n - w});
                pd.explanation = lc_arena_strndup(&box->arena, ex.p, ex.n);
                if (!pd.explanation) {
                    rc = lc_oom(ctx);
                    goto fail;
                }
            }
            continue;
        }

        if (ln.n >= 2 && ln.p[0] == '=' && ln.p[1] == '>') {
            span expected = trim((span){ln.p + 2, ln.n - 2});
            if (!OPEN && !(mode == MODE_REGEN && pd.gen)) {
                rc = parse_error(ctx, filename, lineno, "'=>' with no input "
                                 "lines before it");
                goto fail;
            }
            if (expected.n == 0 && mode != MODE_REGEN) {
                rc = parse_error(ctx, filename, lineno, "nothing after '=>'");
                goto fail;
            }
            if (end_case(&box->arena, &items, &nitems, &capitems, args, nargs,
                         expected.n ? &expected : NULL, lineno, &pd,
                         filename) != LC_OK) {
                rc = lc_oom(ctx);
                goto fail;
            }
            nargs = 0;
            noinput = 0;
            memset(&pd, 0, sizeof pd);
            continue;
        }

        /* "()": a case with no inputs, for a signature without
         * parameters. It must be the case's only input line. */
        if (ln.n == 2 && ln.p[0] == '(' && ln.p[1] == ')') {
            if (OPEN) {
                rc = parse_error(ctx, filename, lineno, "'()' must be the "
                                 "only input line of its case");
                goto fail;
            }
            noinput = 1;
            pd.line = pd.last_line = lineno;
            continue;
        }
        if (noinput) {
            rc = parse_error(ctx, filename, lineno, "input line after '()' "
                             "(a case with no inputs has no other input "
                             "lines)");
            goto fail;
        }

        /* An input line: one literal, kept as text. */
        if (!nargs)
            pd.line = lineno;
        pd.last_line = lineno;
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
    if ((OPEN && mode != MODE_TESTS) || (mode == MODE_REGEN && pd.gen)) {
        FLUSH();
    } else if (OPEN) {
        rc = parse_error(ctx, filename, pd.line, "case has no '=>' line");
        goto fail;
    }
    if (pd.example) {
        rc = parse_error(ctx, filename, pd.example_line, "@example is not "
                         "followed by a case");
        goto fail;
    }
    if (pd.gen) {
        rc = parse_error(ctx, filename, pd.gen_line, "@gen is not followed "
                         "by a case (run 'lc check --regen' to make it)");
        goto fail;
    }
#undef OPEN
#undef FLUSH

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

int lc_parse_regen_cases(lc_ctx *ctx, const char *text, size_t len,
                         const char *filename, lc_cases **out)
{
    return parse_cases(ctx, text, len, filename, MODE_REGEN, out);
}

int lc_parse_user_cases(lc_ctx *ctx, const char *text, size_t len,
                        const char *filename, lc_cases **out)
{
    return parse_cases(ctx, text, len, filename, MODE_USER, out);
}

int lc_parse_cases(lc_ctx *ctx, const char *text, size_t len,
                   const char *filename, lc_cases **out)
{
    return parse_cases(ctx, text, len, filename, MODE_TESTS, out);
}

int lc_load_user_cases(lc_ctx *ctx, const char *path, lc_cases **out)
{
    lc_str text = LC_STR_INIT;
    struct stat st;
    int rc;

    if (!out)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_load_user_cases: no "
                                              "output"));
    *out = NULL;
    if (!path)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_load_user_cases: no "
                                              "path"));
    /* Only a regular file, as for tests.txt. */
    if (stat(path, &st) == 0 && !S_ISREG(st.st_mode))
        return error_status(ctx, lc_set_error(ctx, LC_ERR_IO, "cannot read "
                                              "'%s': not a regular file",
                                              path));
    rc = lc_read_file(ctx, path, LC_MAX_TESTS_TXT, &text);
    if (rc == LC_OK)
        rc = parse_cases(ctx, text.data, text.len, path, MODE_USER, out);
    else
        rc = error_status(ctx, rc);
    lc_str_free(&text);
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

/* ------------------------------------------------------------------ */
/* Typed values of a case                                              */
/* ------------------------------------------------------------------ */

/*
 * Rewrites the last error, a literal's "column N: ..." message, as
 * "<dir>/tests.txt:<line>: <what>: column N: ...".
 */
static int case_error(lc_ctx *ctx, const lc_problem *p, const lc_case *c,
                      const char *what, int rc)
{
    if (rc != LC_ERR_PARSE)
        return rc;
    if (c->file)
        return error_status(ctx, lc_set_error(ctx, rc, "%s:%d: %s: %s",
                                              c->file, c->line, what,
                                              lc_last_error(ctx)));
    return error_status(ctx, lc_set_error(ctx, rc, "%s/tests.txt:%d: %s: %s",
                                          p->dir, c->line, what,
                                          lc_last_error(ctx)));
}

int lc_case_values(lc_ctx *ctx, const lc_problem *p, const lc_case *c,
                   lc_pool *pool, const lc_value **args,
                   const lc_value **expected)
{
    const lc_sig *sig;
    lc_value *vals;
    size_t i, want;
    int rc;

    if (!p || !p->dir || !c || !pool || !args || !expected)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_case_values: bad "
                                              "arguments"));
    *args = NULL;
    *expected = NULL;
    sig = p->sig;
    want = sig ? sig->nparams + p->nhidden : c->nargs;
    if (c->nargs != want) {
        lc_str names = LC_STR_INIT;
        for (i = 0; i < want; i++) {
            const lc_param *pr = i < sig->nparams ? &sig->params[i]
                                                  : &p->hidden[i - sig->nparams];
            lc_str_printf(&names, "%s%s", i ? ", " : "", pr->name);
        }
        if (names.failed) {
            lc_str_free(&names);
            return lc_oom(ctx);
        }
        if (c->file)
            rc = lc_set_error(ctx, LC_ERR_PARSE, "%s:%d: case has %lu "
                              "input%s, expected %lu%s%s%s", c->file, c->line,
                              (unsigned long)c->nargs,
                              c->nargs == 1 ? "" : "s", (unsigned long)want,
                              want ? " (" : "", lc_str_cstr(&names),
                              want ? ")" : "");
        else
            rc = lc_set_error(ctx, LC_ERR_PARSE, "%s/tests.txt:%d: case has "
                              "%lu input%s, expected %lu%s%s%s", p->dir,
                              c->line, (unsigned long)c->nargs,
                              c->nargs == 1 ? "" : "s", (unsigned long)want,
                              want ? " (" : "", lc_str_cstr(&names),
                              want ? ")" : "");
        lc_str_free(&names);
        return error_status(ctx, rc);
    }

    vals = lc_arena_array(&pool->arena, c->nargs, sizeof *vals);
    if (!vals)
        return lc_oom(ctx);
    for (i = 0; i < c->nargs; i++) {
        const lc_param *pr = NULL;
        const lc_value *v;
        char label[32];

        if (sig)
            pr = i < sig->nparams ? &sig->params[i]
                                  : &p->hidden[i - sig->nparams];
        rc = lc_value_parse(ctx, pool, pr ? &pr->type : NULL, c->args[i],
                            strlen(c->args[i]), &v);
        if (rc != LC_OK) {
            snprintf(label, sizeof label, "input %lu", (unsigned long)i + 1);
            return case_error(ctx, p, c, pr ? pr->name : label, rc);
        }
        vals[i] = *v;
    }
    if (c->expected) {
        rc = lc_value_parse(ctx, pool,
                            sig && sig->ret.kind != LC_T_VOID ? &sig->ret
                                                              : NULL,
                            c->expected, strlen(c->expected), expected);
        if (rc != LC_OK)
            return case_error(ctx, p, c, "expected output", rc);
    }
    *args = vals;
    return LC_OK;
}
