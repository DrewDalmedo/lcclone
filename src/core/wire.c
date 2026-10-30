#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * wire.c: the wire format between lc and the harness (docs/wire.md):
 * whitespace-separated tokens, length-prefixed strings, counted arrays.
 */

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "internal.h"

#define TOKEN_QUOTE 20 /* longest part of a bad token quoted in a message */

/* The status to return after setting the last error (see cases.c). */
static int error_status(const lc_ctx *ctx, int rc)
{
    return ctx->err_nomem ? LC_ERR_NOMEM : rc;
}

static int valid_type(const lc_type *t)
{
    return t && (unsigned)t->kind <= LC_T_NARY && t->dims >= 0 &&
           t->dims <= LC_MAX_DIMS && (t->kind != LC_T_VOID || t->dims == 0);
}

static int is_node(lc_kind k)
{
    return k == LC_T_LIST || k == LC_T_TREE || k == LC_T_NARY;
}

/* ------------------------------------------------------------------ */
/* Encoding                                                            */
/* ------------------------------------------------------------------ */

static int in_range(lc_kind k, long long v)
{
    switch (k) {
    case LC_T_INT:
        return v >= INT_MIN && v <= INT_MAX;
    case LC_T_UINT32:
        return v >= 0 && v <= (long long)UINT32_MAX;
    default:
        return 1;
    }
}

/* Appends the tokens of v; 0 when v doesn't fit (kind, dims). */
static int encode(lc_kind kind, int dims, const lc_value *v, lc_str *out)
{
    char buf[LC_DOUBLE_BUF];
    size_t i;

    if (dims > 0 || is_node(kind)) {
        if (v->kind != LC_V_ARRAY)
            return 0;
        lc_str_printf(out, "%lu", (unsigned long)v->len);
        for (i = 0; i < v->len; i++) {
            const lc_value *it = &v->u.items[i];
            lc_str_putc(out, ' ');
            if (dims > 0) {
                if (!encode(kind, dims - 1, it, out))
                    return 0;
            } else if (it->kind == LC_V_NULL && kind != LC_T_LIST) {
                lc_str_putc(out, 'n');
            } else if (it->kind != LC_V_INT || !in_range(LC_T_INT, it->u.i)) {
                return 0;
            } else {
                lc_str_printf(out, "%lld", it->u.i);
            }
        }
        return 1;
    }
    switch (kind) {
    case LC_T_INT:
    case LC_T_LONG:
    case LC_T_UINT32:
        if (v->kind != LC_V_INT || !in_range(kind, v->u.i))
            return 0;
        lc_str_printf(out, "%lld", v->u.i);
        return 1;
    case LC_T_DOUBLE:
        if (v->kind != LC_V_FLOAT)
            return 0;
        lc_fmt_double(v->u.f, buf);
        lc_str_puts(out, buf);
        return 1;
    case LC_T_BOOL:
        if (v->kind != LC_V_BOOL)
            return 0;
        lc_str_putc(out, v->u.b ? '1' : '0');
        return 1;
    case LC_T_CHAR:
        if (v->kind != LC_V_STRING || v->len != 1)
            return 0;
        lc_str_printf(out, "%u", (unsigned)(unsigned char)v->u.s[0]);
        return 1;
    case LC_T_STRING:
        if (v->kind != LC_V_STRING)
            return 0;
        lc_str_printf(out, "%lu:", (unsigned long)v->len);
        lc_str_append(out, v->u.s, v->len);
        return 1;
    default:
        return 0;
    }
}

int lc_wire_encode(lc_ctx *ctx, const lc_type *type, const lc_value *v,
                   lc_str *out)
{
    char name[LC_TYPE_NAME_MAX];

    if (!valid_type(type) || !out || (!v && type->kind != LC_T_VOID))
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_wire_encode: bad "
                                              "arguments"));
    if (type->kind == LC_T_VOID)
        return LC_OK;
    if (!encode(type->kind, type->dims, v, out))
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_wire_encode: the value "
                                              "doesn't fit %s",
                                              lc_type_name(*type, name)));
    return out->failed ? lc_oom(ctx) : LC_OK;
}

/* ------------------------------------------------------------------ */
/* Decoding                                                            */
/* ------------------------------------------------------------------ */

typedef struct decoder {
    lc_ctx *ctx;
    lc_arena *a;
    const char *s;
    size_t n, i;
} decoder;

static int is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static void skip_space(decoder *D)
{
    while (D->i < D->n && is_space((unsigned char)D->s[D->i]))
        D->i++;
}

static int derr(decoder *D, size_t at, const char *fmt, ...) LC_PRINTF(3, 4);

/* Sets "byte N: <message>" (N counts from 1) and returns LC_ERR_PARSE. */
static int derr(decoder *D, size_t at, const char *fmt, ...)
{
    lc_str msg = LC_STR_INIT;
    va_list ap;
    int rc;

    lc_str_printf(&msg, "byte %lu: ", (unsigned long)(at + 1));
    va_start(ap, fmt);
    lc_str_vprintf(&msg, fmt, ap);
    va_end(ap);
    if (msg.failed) {
        lc_str_free(&msg);
        return lc_oom(D->ctx);
    }
    rc = error_status(D->ctx, lc_set_error(D->ctx, LC_ERR_PARSE, "%s",
                                           msg.data));
    lc_str_free(&msg);
    return rc;
}

/* The next token: a run of non-space bytes. Empty at the end. */
static size_t token(decoder *D, const char **p)
{
    size_t start;

    skip_space(D);
    start = D->i;
    while (D->i < D->n && !is_space((unsigned char)D->s[D->i]))
        D->i++;
    *p = D->s + start;
    return D->i - start;
}

/* Error for a bad token at tok[0..len). */
static int bad_token(decoder *D, const char *tok, size_t len,
                     const char *what)
{
    size_t at = (size_t)(tok - D->s);

    if (len == 0)
        return derr(D, at, "expected %s, got the end of the result", what);
    return derr(D, at, "expected %s, got '%.*s%s'", what,
                (int)(len > TOKEN_QUOTE ? TOKEN_QUOTE : len), tok,
                len > TOKEN_QUOTE ? "..." : "");
}

/* An integer token from lo to hi. */
static int read_int(decoder *D, long long lo, long long hi, const char *what,
                    long long *out)
{
    const char *tok;
    size_t len = token(D, &tok), i = 0;
    long long v = 0;
    int neg = 0;

    if (len > 0 && tok[0] == '-') {
        neg = 1;
        i = 1;
    }
    if (i >= len)
        return bad_token(D, tok, len, what);
    for (; i < len; i++) {
        int d = tok[i] - '0';
        if (d < 0 || d > 9)
            return bad_token(D, tok, len, what);
        if (v < (LLONG_MIN + d) / 10)
            return bad_token(D, tok, len, what);
        v = v * 10 - d;
    }
    if (!neg) {
        if (v == LLONG_MIN)
            return bad_token(D, tok, len, what);
        v = -v;
    }
    if (v < lo || v > hi)
        return bad_token(D, tok, len, what);
    *out = v;
    return LC_OK;
}

static int read_double(decoder *D, double *out)
{
    const char *tok;
    size_t len = token(D, &tok);
    int is_int;

    if (len == 3 && memcmp(tok, "inf", 3) == 0) {
        *out = INFINITY;
        return LC_OK;
    }
    if (len == 4 && memcmp(tok, "-inf", 4) == 0) {
        *out = -INFINITY;
        return LC_OK;
    }
    if (len == 3 && memcmp(tok, "nan", 3) == 0) {
        *out = NAN;
        return LC_OK;
    }
    if (len == 0 || lc_scan_number(tok, len, &is_int) != len ||
        lc_strtod(tok, len, out) != 0)
        return bad_token(D, tok, len, "a double");
    return LC_OK;
}

/* A "len:bytes" string. */
static int read_string(decoder *D, lc_value *out)
{
    size_t start, len = 0, at;
    char *d;

    skip_space(D);
    at = start = D->i;
    while (D->i < D->n && D->s[D->i] >= '0' && D->s[D->i] <= '9') {
        if (len > D->n / 10) /* longer than any text could hold */
            return derr(D, at, "string length is too large");
        len = len * 10 + (size_t)(D->s[D->i] - '0');
        D->i++;
    }
    if (D->i == start || D->i >= D->n || D->s[D->i] != ':') {
        const char *tok;
        D->i = start;
        return bad_token(D, D->s + start, token(D, &tok),
                         "a string ('length:bytes')");
    }
    D->i++;
    if (len > D->n - D->i)
        return derr(D, at, "string length %lu is longer than the rest of "
                    "the result", (unsigned long)len);
    d = lc_arena_alloc(D->a, len + 1);
    if (!d)
        return lc_oom(D->ctx);
    memcpy(d, D->s + D->i, len);
    d[len] = '\0';
    D->i += len;
    if (D->i < D->n && !is_space((unsigned char)D->s[D->i]))
        return derr(D, D->i, "expected a space after the string's %lu "
                    "bytes", (unsigned long)len);
    out->kind = LC_V_STRING;
    out->len = len;
    out->u.s = d;
    return LC_OK;
}

static int decode(decoder *D, lc_kind kind, int dims, lc_value *out);

static int decode_array(decoder *D, lc_kind kind, int dims, lc_value *out)
{
    long long count;
    size_t at, i, nonnull = 0, group = 0, n;
    lc_value *items;
    int rc;

    skip_space(D);
    at = D->i;
    rc = read_int(D, 0, LLONG_MAX, "an array length", &count);
    if (rc != LC_OK)
        return rc;
    /* Each item takes at least a separator and one byte. */
    if ((unsigned long long)count > (D->n - D->i) / 2)
        return derr(D, at, "array length %lld is longer than the rest of "
                    "the result", count);
    n = (size_t)count;
    items = lc_arena_array(D->a, n ? n : 1, sizeof *items);
    if (!items)
        return lc_oom(D->ctx);
    for (i = 0; i < n; i++) {
        if (dims > 0) {
            rc = decode(D, kind, dims - 1, &items[i]);
            if (rc != LC_OK)
                return rc;
            continue;
        }
        /* An item of a ListNode, TreeNode or NaryNode. */
        skip_space(D);
        if (kind != LC_T_LIST && D->i < D->n && D->s[D->i] == 'n' &&
            (D->i + 1 == D->n || is_space((unsigned char)D->s[D->i + 1]))) {
            items[i].kind = LC_V_NULL;
            D->i++;
        } else {
            rc = read_int(D, INT_MIN, INT_MAX,
                          kind == LC_T_LIST ? "a node value"
                                            : "a node value or 'n'",
                          &items[i].u.i);
            if (rc != LC_OK)
                return rc;
            items[i].kind = LC_V_INT;
        }
        if (kind != LC_T_LIST) {
            const char *why = lc_tree_item_check(
                kind, i, items[i].kind == LC_V_NULL, nonnull, &group);
            if (why)
                return derr(D, at, "item %lu of the tree: %s",
                            (unsigned long)i, why);
            if (items[i].kind != LC_V_NULL)
                nonnull++;
        }
    }
    if (dims == 0 && kind != LC_T_LIST)
        while (n > 0 && items[n - 1].kind == LC_V_NULL)
            n--;
    out->kind = LC_V_ARRAY;
    out->len = n;
    out->u.items = items;
    return LC_OK;
}

static int decode(decoder *D, lc_kind kind, int dims, lc_value *out)
{
    long long v;
    int rc;

    memset(out, 0, sizeof *out);
    if (dims > 0 || is_node(kind))
        return decode_array(D, kind, dims, out);
    switch (kind) {
    case LC_T_INT:
        rc = read_int(D, INT_MIN, INT_MAX, "an int", &out->u.i);
        out->kind = LC_V_INT;
        return rc;
    case LC_T_LONG:
        rc = read_int(D, LLONG_MIN, LLONG_MAX, "a long", &out->u.i);
        out->kind = LC_V_INT;
        return rc;
    case LC_T_UINT32:
        rc = read_int(D, 0, UINT32_MAX, "a uint32", &out->u.i);
        out->kind = LC_V_INT;
        return rc;
    case LC_T_DOUBLE:
        out->kind = LC_V_FLOAT;
        return read_double(D, &out->u.f);
    case LC_T_BOOL:
        rc = read_int(D, 0, 1, "a bool (0 or 1)", &v);
        out->kind = LC_V_BOOL;
        out->u.b = (int)v;
        return rc;
    case LC_T_CHAR: {
        char *d;
        rc = read_int(D, 0, 255, "a char (0 to 255)", &v);
        if (rc != LC_OK)
            return rc;
        d = lc_arena_alloc(D->a, 2);
        if (!d)
            return lc_oom(D->ctx);
        d[0] = (char)(unsigned char)v;
        d[1] = '\0';
        out->kind = LC_V_STRING;
        out->len = 1;
        out->u.s = d;
        return LC_OK;
    }
    case LC_T_STRING:
        return read_string(D, out);
    default:
        return error_status(D->ctx, lc_set_error(D->ctx, LC_ERR_INVALID,
                                                 "lc_wire_decode: bad "
                                                 "type"));
    }
}

int lc_wire_decode(lc_ctx *ctx, lc_pool *pool, const lc_type *type,
                   const char *text, size_t len, size_t *pos,
                   const lc_value **out)
{
    decoder D;
    lc_value *v;
    int rc;

    if (!valid_type(type) || !pool || !pos || !out || (!text && len) ||
        *pos > len)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_wire_decode: bad "
                                              "arguments"));
    *out = NULL;
    v = lc_arena_zalloc(&pool->arena, sizeof *v);
    if (!v)
        return lc_oom(ctx);
    if (type->kind == LC_T_VOID) {
        *out = v; /* LC_V_NULL; nothing is read */
        return LC_OK;
    }
    D.ctx = ctx;
    D.a = &pool->arena;
    D.s = text ? text : "";
    D.n = len;
    D.i = *pos;
    rc = decode(&D, type->kind, type->dims, v);
    if (rc != LC_OK)
        return rc;
    *pos = D.i;
    *out = v;
    return LC_OK;
}
