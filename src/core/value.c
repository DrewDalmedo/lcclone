#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * value.c: literals (docs/problem-format.md, "Literals"): parsing them,
 * checking them against a type, and printing values back.
 */

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

#define GOT_MAX 30         /* longest part of a number quoted in a message */
#define NUMBER_MAX 400     /* longest number lc_strtod converts */

/* ------------------------------------------------------------------ */
/* Pools                                                               */
/* ------------------------------------------------------------------ */

lc_pool *lc_pool_new(void)
{
    return calloc(1, sizeof(lc_pool));
}

void lc_pool_free(lc_pool *pool)
{
    if (!pool)
        return;
    lc_arena_free(&pool->arena);
    free(pool);
}

/* ------------------------------------------------------------------ */
/* Numbers                                                             */
/* ------------------------------------------------------------------ */

static int is_digit(int c)
{
    return c >= '0' && c <= '9';
}

size_t lc_scan_number(const char *p, size_t n, int *is_int)
{
    size_t i = 0;

    *is_int = 1;
    if (i < n && p[i] == '-')
        i++;
    if (i >= n || !is_digit((unsigned char)p[i]))
        return 0;
    if (p[i] == '0') {
        i++;
        if (i < n && is_digit((unsigned char)p[i]))
            return 0; /* a leading zero */
    } else {
        while (i < n && is_digit((unsigned char)p[i]))
            i++;
    }
    if (i < n && p[i] == '.') {
        i++;
        if (i >= n || !is_digit((unsigned char)p[i]))
            return 0;
        while (i < n && is_digit((unsigned char)p[i]))
            i++;
        *is_int = 0;
    }
    if (i < n && (p[i] == 'e' || p[i] == 'E')) {
        i++;
        if (i < n && (p[i] == '+' || p[i] == '-'))
            i++;
        if (i >= n || !is_digit((unsigned char)p[i]))
            return 0;
        while (i < n && is_digit((unsigned char)p[i]))
            i++;
        *is_int = 0;
    }
    return i;
}

/*
 * The current locale's decimal point, as printf writes it. It is asked
 * for each time rather than cached: a front-end may change the locale, and
 * the core keeps no global state.
 */
static size_t locale_radix(char *buf, size_t size)
{
    char probe[32];
    const char *five;
    size_t len;

    snprintf(probe, sizeof probe, "%.1f", 0.5);
    five = strrchr(probe, '5');
    if (!five || five <= probe + 1) {
        buf[0] = '.';
        return 1;
    }
    len = (size_t)(five - (probe + 1));
    if (len >= size)
        len = size - 1;
    memcpy(buf, probe + 1, len);
    return len;
}

int lc_strtod(const char *p, size_t n, double *out)
{
    char buf[NUMBER_MAX + 16], radix[8], *end;
    size_t rlen = locale_radix(radix, sizeof radix), i, j = 0;
    double v;

    if (n > NUMBER_MAX)
        return -1;
    for (i = 0; i < n; i++) {
        if (j + rlen >= sizeof buf)
            return -1; /* only if the caller broke the contract */
        if (p[i] == '.') {
            memcpy(buf + j, radix, rlen);
            j += rlen;
        } else {
            buf[j++] = p[i];
        }
    }
    buf[j] = '\0';
    v = strtod(buf, &end);
    if (end != buf + j || isinf(v) || isnan(v))
        return -1;
    *out = v;
    return 0;
}

void lc_fmt_double(double v, char *buf)
{
    char tmp[LC_DOUBLE_BUF + 16];
    size_t i, j = 0;
    int prec, in_radix = 0;

    if (isnan(v)) {
        strcpy(buf, "nan");
        return;
    }
    if (isinf(v)) {
        strcpy(buf, v < 0 ? "-inf" : "inf");
        return;
    }
    for (prec = 15; prec < 17; prec++) {
        snprintf(tmp, sizeof tmp, "%.*g", prec, v);
        if (strtod(tmp, NULL) == v)
            break;
    }
    if (prec == 17)
        snprintf(tmp, sizeof tmp, "%.17g", v);
    /* %g writes only digits, signs, 'e' and the decimal point, which the
     * locale may make something other than '.' (even several bytes). */
    for (i = 0; tmp[i] && j < LC_DOUBLE_BUF - 1; i++) {
        char c = tmp[i];
        if (is_digit((unsigned char)c) || c == '-' || c == '+' || c == 'e') {
            buf[j++] = c;
            in_radix = 0;
        } else if (!in_radix) {
            buf[j++] = '.';
            in_radix = 1;
        }
    }
    buf[j] = '\0';
}

/* ------------------------------------------------------------------ */
/* UTF-8                                                               */
/* ------------------------------------------------------------------ */

/* Length of the valid UTF-8 sequence at p[0..n), or 0 if it isn't one. */
static size_t utf8_len(const unsigned char *p, size_t n)
{
    unsigned c = p[0];
    size_t len, i;
    unsigned long cp;

    if (c < 0x80)
        return 1;
    if (c >= 0xC2 && c <= 0xDF) {
        len = 2;
        cp = c & 0x1F;
    } else if (c >= 0xE0 && c <= 0xEF) {
        len = 3;
        cp = c & 0x0F;
    } else if (c >= 0xF0 && c <= 0xF4) {
        len = 4;
        cp = c & 0x07;
    } else {
        return 0;
    }
    if (n < len)
        return 0;
    for (i = 1; i < len; i++) {
        if ((p[i] & 0xC0) != 0x80)
            return 0;
        cp = (cp << 6) | (p[i] & 0x3F);
    }
    if ((len == 3 && cp < 0x800) || (len == 4 && cp < 0x10000) ||
        cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF))
        return 0;
    return len;
}

static size_t put_utf8(char *d, unsigned long cp)
{
    if (cp < 0x80) {
        d[0] = (char)cp;
        return 1;
    }
    if (cp < 0x800) {
        d[0] = (char)(0xC0 | (cp >> 6));
        d[1] = (char)(0x80 | (cp & 0x3F));
        return 2;
    }
    if (cp < 0x10000) {
        d[0] = (char)(0xE0 | (cp >> 12));
        d[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        d[2] = (char)(0x80 | (cp & 0x3F));
        return 3;
    }
    d[0] = (char)(0xF0 | (cp >> 18));
    d[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    d[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
    d[3] = (char)(0x80 | (cp & 0x3F));
    return 4;
}

/* ------------------------------------------------------------------ */
/* Parser                                                              */
/* ------------------------------------------------------------------ */

/* What the parser expects at a value. */
typedef struct want {
    int any;      /* any literal at all; the other fields are unused */
    lc_kind kind;
    int dims;
    int nullable; /* null is allowed too (items of a tree) */
} want;

typedef struct parser {
    lc_arena *a;
    const char *s;
    size_t n, i;
    lc_value *stack; /* items of the arrays being parsed */
    size_t nstack, capstack;
    lc_str *err;
    int depth;
} parser;

static int perr(parser *P, size_t at, const char *fmt, ...) LC_PRINTF(3, 4);

/* Sets "column N: <message>" for offset `at` and returns LC_ERR_PARSE. */
static int perr(parser *P, size_t at, const char *fmt, ...)
{
    va_list ap;

    lc_str_clear(P->err);
    lc_str_printf(P->err, "column %lu: ", (unsigned long)(at + 1));
    va_start(ap, fmt);
    lc_str_vprintf(P->err, fmt, ap);
    va_end(ap);
    return P->err->failed ? LC_ERR_NOMEM : LC_ERR_PARSE;
}

static void skip_ws(parser *P)
{
    while (P->i < P->n) {
        char c = P->s[P->i];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
            break;
        P->i++;
    }
}

static int is_word_char(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || is_digit(c) ||
           c == '_';
}

/* 1 if `word` is at the current position, not followed by a letter. */
static int at_word(const parser *P, const char *word)
{
    size_t len = strlen(word);

    return P->n - P->i >= len && memcmp(P->s + P->i, word, len) == 0 &&
           (P->i + len == P->n ||
            !is_word_char((unsigned char)P->s[P->i + len]));
}

/* Error for a character no literal can start with. */
static int bad_char(parser *P)
{
    unsigned char c;

    if (P->i >= P->n)
        return perr(P, P->i, "expected a value");
    c = (unsigned char)P->s[P->i];
    if (c >= 0x21 && c < 0x7F)
        return perr(P, P->i, "unexpected character '%c'", c);
    return perr(P, P->i, "unexpected byte 0x%02X", c);
}

/* The length of the run of number-like characters at the position, to
 * quote a bad number. */
static size_t number_run(const parser *P)
{
    size_t j = P->i;

    while (j < P->n && j - P->i < GOT_MAX) {
        char c = P->s[j];
        if (!is_digit((unsigned char)c) && c != '-' && c != '+' &&
            c != '.' && c != 'e' && c != 'E')
            break;
        j++;
    }
    return j - P->i;
}

static void want_name(want w, char *buf)
{
    lc_type t;

    if (w.nullable) {
        strcpy(buf, "int or null");
        return;
    }
    t.kind = w.kind;
    t.dims = w.dims;
    lc_type_name(t, buf);
}

/* Error for a well-formed value of the wrong kind: "expected int, got a
 * string". */
static int mismatch(parser *P, want w)
{
    char name[LC_TYPE_NAME_MAX];
    const char *got = NULL;
    size_t at = P->i;
    char c = at < P->n ? P->s[at] : '\0';

    want_name(w, name);
    if (c == '"')
        got = "a string";
    else if (c == '[')
        got = "an array";
    else if (at_word(P, "true"))
        got = "true";
    else if (at_word(P, "false"))
        got = "false";
    else if (at_word(P, "null"))
        got = "null";
    if (got)
        return perr(P, at, "expected %s, got %s", name, got);
    if (c == '-' || is_digit((unsigned char)c)) {
        int is_int;
        size_t len = lc_scan_number(P->s + at, P->n - at, &is_int);
        if (len == 0)
            return perr(P, at, "invalid number '%.*s'", (int)number_run(P),
                        P->s + at);
        return perr(P, at, "expected %s, got %.*s%s", name,
                    (int)(len > GOT_MAX ? GOT_MAX : len), P->s + at,
                    len > GOT_MAX ? "..." : "");
    }
    return bad_char(P);
}

/* Reads a number at the position; is_int says whether it had no
 * fraction or exponent. */
static int scan_number(parser *P, size_t *len, int *is_int)
{
    *len = lc_scan_number(P->s + P->i, P->n - P->i, is_int);
    if (*len == 0)
        return perr(P, P->i, "invalid number '%.*s'", (int)number_run(P),
                    P->s + P->i);
    return LC_OK;
}

/* The integer p[0..n) (already scanned) if it fits in a long long. */
static int to_ll(const char *p, size_t n, long long *out)
{
    long long v = 0;
    size_t i = 0;
    int neg = 0;

    if (p[0] == '-') {
        neg = 1;
        i = 1;
    }
    /* Accumulate negatively, so LLONG_MIN fits. */
    for (; i < n; i++) {
        int d = p[i] - '0';
        if (v < (LLONG_MIN + d) / 10)
            return -1;
        v = v * 10 - d;
    }
    if (!neg) {
        if (v == LLONG_MIN)
            return -1;
        v = -v;
    }
    *out = v;
    return 0;
}

static int parse_int(parser *P, want w, lc_value *out)
{
    char name[LC_TYPE_NAME_MAX];
    size_t at = P->i, len;
    long long v, lo = INT_MIN, hi = INT_MAX;
    int is_int, rc;

    rc = scan_number(P, &len, &is_int);
    if (rc != LC_OK)
        return rc;
    if (!is_int)
        return mismatch(P, w);
    if (w.kind == LC_T_LONG) {
        lo = LLONG_MIN;
        hi = LLONG_MAX;
    } else if (w.kind == LC_T_UINT32) {
        lo = 0;
        hi = UINT32_MAX;
    }
    want_name(w, name);
    if (to_ll(P->s + at, len, &v) != 0 || v < lo || v > hi)
        return perr(P, at, "%.*s%s is out of range for %s",
                    (int)(len > GOT_MAX ? GOT_MAX : len), P->s + at,
                    len > GOT_MAX ? "..." : "", w.nullable ? "int" : name);
    out->kind = LC_V_INT;
    out->u.i = v;
    P->i += len;
    return LC_OK;
}

static int parse_float(parser *P, lc_value *out)
{
    size_t at = P->i, len;
    int is_int, rc;
    double v;

    rc = scan_number(P, &len, &is_int);
    if (rc != LC_OK)
        return rc;
    if (lc_strtod(P->s + at, len, &v) != 0)
        return perr(P, at, "%.*s%s is out of range for double",
                    (int)(len > GOT_MAX ? GOT_MAX : len), P->s + at,
                    len > GOT_MAX ? "..." : "");
    out->kind = LC_V_FLOAT;
    out->u.f = v;
    P->i += len;
    return LC_OK;
}

/* An untyped number: an integer if it has no fraction or exponent. */
static int parse_any_number(parser *P, lc_value *out)
{
    size_t at = P->i, len;
    int is_int, rc;

    rc = scan_number(P, &len, &is_int);
    if (rc != LC_OK)
        return rc;
    if (!is_int)
        return parse_float(P, out);
    if (to_ll(P->s + at, len, &out->u.i) != 0)
        return perr(P, at, "integer %.*s%s is out of range",
                    (int)(len > GOT_MAX ? GOT_MAX : len), P->s + at,
                    len > GOT_MAX ? "..." : "");
    out->kind = LC_V_INT;
    P->i += len;
    return LC_OK;
}

static int hex4(const char *p, unsigned long *out)
{
    unsigned long v = 0;
    int k;

    for (k = 0; k < 4; k++) {
        int c = (unsigned char)p[k], d;
        if (is_digit(c))
            d = c - '0';
        else if (c >= 'a' && c <= 'f')
            d = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            d = c - 'A' + 10;
        else
            return -1;
        v = v * 16 + (unsigned long)d;
    }
    *out = v;
    return 0;
}

/* Reads a \uXXXX escape at s[*j] (just after the 'u'), combining a
 * surrogate pair. */
static int parse_u_escape(parser *P, size_t esc, size_t *j, unsigned long *cp)
{
    unsigned long lo;

    if (P->n - *j < 4 || hex4(P->s + *j, cp) != 0)
        return perr(P, esc, "\\u must be followed by four hex digits");
    *j += 4;
    if (*cp >= 0xDC00 && *cp <= 0xDFFF)
        return perr(P, esc, "unpaired surrogate \\u%.4s", P->s + esc + 2);
    if (*cp >= 0xD800 && *cp <= 0xDBFF) {
        if (P->n - *j < 6 || P->s[*j] != '\\' || P->s[*j + 1] != 'u' ||
            hex4(P->s + *j + 2, &lo) != 0 || lo < 0xDC00 || lo > 0xDFFF)
            return perr(P, esc, "unpaired surrogate \\u%.4s",
                        P->s + esc + 2);
        *cp = 0x10000 + ((*cp - 0xD800) << 10) + (lo - 0xDC00);
        *j += 6;
    }
    if (*cp == 0)
        return perr(P, esc, "\\u0000 is not allowed (strings can't hold a "
                    "NUL byte)");
    return LC_OK;
}

static int parse_string(parser *P, lc_value *out)
{
    size_t open = P->i, j, end;
    char *d;
    size_t dn = 0;

    /* Find the closing quote first, to size the copy: unescaping never
     * makes a string longer. */
    for (end = open + 1; end < P->n && P->s[end] != '"'; end++)
        if (P->s[end] == '\\')
            end++;
    if (end >= P->n)
        return perr(P, open, "string has no closing quote");
    d = lc_arena_alloc(P->a, end - open);
    if (!d)
        return LC_ERR_NOMEM;

    for (j = open + 1; j < end;) {
        unsigned char c = (unsigned char)P->s[j];

        if (c == '\\') {
            size_t esc = j;
            unsigned long cp;
            int rc;

            j += 2;
            switch (P->s[esc + 1]) {
            case '"': d[dn++] = '"'; break;
            case '\\': d[dn++] = '\\'; break;
            case '/': d[dn++] = '/'; break;
            case 'b': d[dn++] = '\b'; break;
            case 'f': d[dn++] = '\f'; break;
            case 'n': d[dn++] = '\n'; break;
            case 'r': d[dn++] = '\r'; break;
            case 't': d[dn++] = '\t'; break;
            case 'u':
                rc = parse_u_escape(P, esc, &j, &cp);
                if (rc != LC_OK)
                    return rc;
                dn += put_utf8(d + dn, cp);
                break;
            default:
                if ((unsigned char)P->s[esc + 1] >= 0x21 &&
                    (unsigned char)P->s[esc + 1] < 0x7F)
                    return perr(P, esc, "unknown escape '\\%c'",
                                P->s[esc + 1]);
                return perr(P, esc, "unknown escape");
            }
        } else if (c < 0x20) {
            return perr(P, j, "control character in a string (write it as "
                        "an escape such as \\n or \\u%04X)", c);
        } else {
            size_t len = utf8_len((const unsigned char *)P->s + j, end - j);
            if (len == 0)
                return perr(P, j, "invalid UTF-8 in a string");
            memcpy(d + dn, P->s + j, len);
            dn += len;
            j += len;
        }
    }
    d[dn] = '\0';
    out->kind = LC_V_STRING;
    out->len = dn;
    out->u.s = d;
    P->i = end + 1;
    return LC_OK;
}

static int parse_value(parser *P, want w, lc_value *out);

static int push_item(parser *P, const lc_value *v)
{
    lc_value *tmp = lc_grow(P->stack, &P->capstack, P->nstack + 1,
                            sizeof *P->stack);

    if (!tmp)
        return LC_ERR_NOMEM;
    P->stack = tmp;
    P->stack[P->nstack++] = *v;
    return LC_OK;
}

const char *lc_tree_item_check(lc_kind node, size_t idx, int is_null,
                               size_t nonnull, size_t *group)
{
    if (idx == 0)
        return is_null ? "a tree can't start with null (write [] for an "
                         "empty tree)" : NULL;
    if (node == LC_T_TREE) {
        /* Item idx is child slot idx - 1, of the (idx - 1) / 2-th node. */
        if ((idx - 1) / 2 >= nonnull)
            return "this item has no parent: every node before it already "
                   "has two children";
        return NULL;
    }
    /* N-ary: the root, null, then each node's children, ended by null. */
    if (idx == 1) {
        *group = 0;
        return is_null ? NULL : "an N-ary tree's root must be followed by "
                                "null";
    }
    if (is_null && ++*group >= nonnull)
        return "this null starts the children of a node that doesn't exist";
    return NULL;
}

static int parse_array(parser *P, want elem, lc_kind node, lc_value *out)
{
    size_t open = P->i, base = P->nstack, count, nonnull = 0, group = 0;
    lc_value *items;
    int rc;

    if (P->depth >= LC_VALUE_MAX_DEPTH)
        return perr(P, open, "arrays nested more than %d deep",
                    LC_VALUE_MAX_DEPTH);
    P->depth++;
    P->i++;
    skip_ws(P);
    if (P->i < P->n && P->s[P->i] == ']') {
        P->i++;
    } else {
        for (;;) {
            lc_value v;
            size_t at;

            skip_ws(P);
            at = P->i;
            if (P->i < P->n && P->s[P->i] == ']')
                return perr(P, at, "expected a value after ','");
            memset(&v, 0, sizeof v);
            rc = parse_value(P, elem, &v);
            if (rc != LC_OK)
                return rc;
            if (node == LC_T_TREE || node == LC_T_NARY) {
                const char *why = lc_tree_item_check(
                    node, P->nstack - base, v.kind == LC_V_NULL, nonnull,
                    &group);
                if (why)
                    return perr(P, at, "%s", why);
                if (v.kind != LC_V_NULL)
                    nonnull++;
            }
            rc = push_item(P, &v);
            if (rc != LC_OK)
                return rc;
            skip_ws(P);
            if (P->i < P->n && P->s[P->i] == ',') {
                P->i++;
                continue;
            }
            if (P->i < P->n && P->s[P->i] == ']') {
                P->i++;
                break;
            }
            if (P->i >= P->n)
                return perr(P, open, "'[' is never closed");
            return perr(P, P->i, "expected ',' or ']'");
        }
    }
    P->depth--;

    count = P->nstack - base;
    /* A tree's trailing nulls say nothing; LeetCode leaves them out. */
    if (node == LC_T_TREE || node == LC_T_NARY)
        while (count > 0 && P->stack[base + count - 1].kind == LC_V_NULL)
            count--;
    items = lc_arena_array(P->a, count ? count : 1, sizeof *items);
    if (!items)
        return LC_ERR_NOMEM;
    if (count)
        memcpy(items, P->stack + base, count * sizeof *items);
    P->nstack = base;
    out->kind = LC_V_ARRAY;
    out->len = count;
    out->u.items = items;
    return LC_OK;
}

static int parse_any(parser *P, lc_value *out)
{
    want any;
    char c;

    memset(&any, 0, sizeof any);
    any.any = 1;
    if (P->i >= P->n)
        return bad_char(P);
    c = P->s[P->i];
    if (c == '[')
        return parse_array(P, any, LC_T_VOID, out);
    if (c == '"')
        return parse_string(P, out);
    if (c == '-' || is_digit((unsigned char)c))
        return parse_any_number(P, out);
    if (at_word(P, "true") || at_word(P, "false")) {
        out->kind = LC_V_BOOL;
        out->u.b = c == 't';
        P->i += c == 't' ? 4 : 5;
        return LC_OK;
    }
    if (at_word(P, "null")) {
        out->kind = LC_V_NULL;
        P->i += 4;
        return LC_OK;
    }
    return bad_char(P);
}

static int parse_value(parser *P, want w, lc_value *out)
{
    char c;
    int rc;

    skip_ws(P);
    if (w.any)
        return parse_any(P, out);
    if (P->i >= P->n)
        return bad_char(P);
    c = P->s[P->i];

    if (w.dims > 0 || w.kind == LC_T_LIST || w.kind == LC_T_TREE ||
        w.kind == LC_T_NARY) {
        want elem = w;
        lc_kind node = LC_T_VOID;

        if (c != '[')
            return mismatch(P, w);
        if (w.dims > 0) {
            elem.dims--;
        } else {
            node = w.kind;
            elem.kind = LC_T_INT;
            elem.nullable = node != LC_T_LIST;
        }
        return parse_array(P, elem, node, out);
    }

    if (w.nullable && at_word(P, "null")) {
        out->kind = LC_V_NULL;
        P->i += 4;
        return LC_OK;
    }
    switch (w.kind) {
    case LC_T_INT:
    case LC_T_LONG:
    case LC_T_UINT32:
        if (c != '-' && !is_digit((unsigned char)c))
            return mismatch(P, w);
        return parse_int(P, w, out);
    case LC_T_DOUBLE:
        if (c != '-' && !is_digit((unsigned char)c))
            return mismatch(P, w);
        return parse_float(P, out);
    case LC_T_BOOL:
        if (!at_word(P, "true") && !at_word(P, "false"))
            return mismatch(P, w);
        return parse_any(P, out);
    case LC_T_CHAR: {
        size_t at = P->i;
        if (c != '"')
            return mismatch(P, w);
        rc = parse_string(P, out);
        if (rc != LC_OK)
            return rc;
        if (out->len != 1 || (unsigned char)out->u.s[0] >= 0x80)
            return perr(P, at, "expected char, a string of one ASCII "
                        "character such as \"a\"");
        return LC_OK;
    }
    case LC_T_STRING:
        if (c != '"')
            return mismatch(P, w);
        return parse_string(P, out);
    default:
        return mismatch(P, w);
    }
}

/* The status to return after setting the last error (see cases.c). */
static int error_status(const lc_ctx *ctx, int rc)
{
    return ctx->err_nomem ? LC_ERR_NOMEM : rc;
}

int lc_value_parse(lc_ctx *ctx, lc_pool *pool, const lc_type *type,
                   const char *text, size_t len, const lc_value **out)
{
    parser P;
    lc_str err = LC_STR_INIT;
    lc_value v, *res;
    want w;
    int rc;

    if (!out || !pool || (!text && len))
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_value_parse: bad "
                                              "arguments"));
    *out = NULL;
    memset(&w, 0, sizeof w);
    if (type) {
        if (type->kind == LC_T_VOID || (unsigned)type->kind > LC_T_NARY ||
            type->dims < 0 || type->dims > LC_MAX_DIMS)
            return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                                  "lc_value_parse: no "
                                                  "values have this type"));
        w.kind = type->kind;
        w.dims = type->dims;
    } else {
        w.any = 1;
    }

    memset(&P, 0, sizeof P);
    P.a = &pool->arena;
    P.s = text ? text : "";
    P.n = len;
    P.err = &err;
    memset(&v, 0, sizeof v);
    rc = parse_value(&P, w, &v);
    if (rc == LC_OK) {
        skip_ws(&P);
        if (P.i < P.n)
            rc = perr(&P, P.i, "unexpected text after the value");
    }
    if (rc == LC_OK) {
        res = lc_arena_alloc(P.a, sizeof *res);
        if (res) {
            *res = v;
            *out = res;
        } else {
            rc = LC_ERR_NOMEM;
        }
    }
    free(P.stack);
    if (rc == LC_ERR_PARSE)
        rc = error_status(ctx, lc_set_error(ctx, rc, "%s", err.data));
    else if (rc != LC_OK)
        rc = lc_oom(ctx);
    lc_str_free(&err);
    return rc;
}

/* ------------------------------------------------------------------ */
/* Printer                                                             */
/* ------------------------------------------------------------------ */

static void print_string(const char *s, size_t n, lc_str *out)
{
    size_t i = 0;

    lc_str_putc(out, '"');
    while (i < n) {
        unsigned char c = (unsigned char)s[i];
        size_t len;

        switch (c) {
        case '"': lc_str_puts(out, "\\\""); i++; continue;
        case '\\': lc_str_puts(out, "\\\\"); i++; continue;
        case '\b': lc_str_puts(out, "\\b"); i++; continue;
        case '\f': lc_str_puts(out, "\\f"); i++; continue;
        case '\n': lc_str_puts(out, "\\n"); i++; continue;
        case '\r': lc_str_puts(out, "\\r"); i++; continue;
        case '\t': lc_str_puts(out, "\\t"); i++; continue;
        default: break;
        }
        if (c < 0x20 || c == 0x7F) {
            lc_str_printf(out, "\\u%04X", c);
            i++;
            continue;
        }
        len = utf8_len((const unsigned char *)s + i, n - i);
        if (len == 0) {
            lc_str_printf(out, "\\x%02X", c);
            i++;
            continue;
        }
        lc_str_append(out, s + i, len);
        i += len;
    }
    lc_str_putc(out, '"');
}

static void print_value(const lc_value *v, lc_str *out)
{
    char buf[LC_DOUBLE_BUF];
    size_t i;

    switch (v->kind) {
    case LC_V_NULL:
        lc_str_puts(out, "null");
        break;
    case LC_V_BOOL:
        lc_str_puts(out, v->u.b ? "true" : "false");
        break;
    case LC_V_INT:
        lc_str_printf(out, "%lld", v->u.i);
        break;
    case LC_V_FLOAT:
        if (isnan(v->u.f)) {
            lc_str_puts(out, "NaN");
        } else if (isinf(v->u.f)) {
            lc_str_puts(out, v->u.f < 0 ? "-Infinity" : "Infinity");
        } else {
            lc_fmt_double(v->u.f, buf);
            lc_str_puts(out, buf);
            if (!strchr(buf, '.') && !strchr(buf, 'e'))
                lc_str_puts(out, ".0");
        }
        break;
    case LC_V_STRING:
        print_string(v->u.s, v->len, out);
        break;
    case LC_V_ARRAY:
        lc_str_putc(out, '[');
        for (i = 0; i < v->len; i++) {
            if (i)
                lc_str_putc(out, ',');
            print_value(&v->u.items[i], out);
        }
        lc_str_putc(out, ']');
        break;
    default:
        lc_str_puts(out, "?");
        break;
    }
}

int lc_value_print(const lc_value *v, lc_str *out)
{
    if (!v || !out)
        return LC_ERR_INVALID;
    print_value(v, out);
    return out->failed ? LC_ERR_NOMEM : LC_OK;
}
