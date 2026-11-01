#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * types.c: the type table, and the problem.md fields built on it:
 * `signature:`, `hidden:`, `check:` and `compare:`
 * (docs/problem-format.md).
 */

#include <stdio.h>
#include <string.h>

#include "internal.h"

#define QUOTE_MAX 60 /* longest part of a value quoted in a message */

typedef struct span {
    const char *p;
    size_t n;
} span;

/* ------------------------------------------------------------------ */
/* The type table                                                      */
/* ------------------------------------------------------------------ */

static const struct kind_info {
    const char *name;
    int max_dims; /* how many [] it may take */
} kinds[] = {
    /* In lc_kind order. */
    {"void", 0},
    {"int", LC_MAX_DIMS},
    {"long", LC_MAX_DIMS},
    {"uint32", LC_MAX_DIMS},
    {"double", LC_MAX_DIMS},
    {"bool", LC_MAX_DIMS},
    {"char", LC_MAX_DIMS},
    {"string", LC_MAX_DIMS},
    {"ListNode", 1},
    {"TreeNode", 1},
    {"NaryNode", 1},
};

#define NKINDS (sizeof kinds / sizeof kinds[0])

enum { TYPE_OK, TYPE_UNKNOWN, TYPE_UNSUPPORTED };

/* Looks up a type name; TYPE_UNSUPPORTED means a known kind with too
 * many "[]". */
static int lookup_type(span s, lc_type *out)
{
    size_t base = s.n, k;
    int dims = 0;

    while (base >= 2 && s.p[base - 2] == '[' && s.p[base - 1] == ']') {
        base -= 2;
        if (dims < LC_MAX_DIMS + 1)
            dims++;
    }
    for (k = 0; k < NKINDS; k++) {
        if (strlen(kinds[k].name) == base &&
            memcmp(kinds[k].name, s.p, base) == 0) {
            if (dims > kinds[k].max_dims)
                return TYPE_UNSUPPORTED;
            out->kind = (lc_kind)k;
            out->dims = dims;
            return TYPE_OK;
        }
    }
    return TYPE_UNKNOWN;
}

int lc_type_parse(const char *s, size_t n, lc_type *out)
{
    span sp;
    lc_type t;

    if (!s || !out)
        return LC_ERR_INVALID;
    sp.p = s;
    sp.n = n;
    if (lookup_type(sp, &t) != TYPE_OK)
        return LC_ERR_PARSE;
    *out = t;
    return LC_OK;
}

const char *lc_type_name(lc_type t, char *buf)
{
    const char *base = "?";
    size_t len;
    int d;

    if ((unsigned)t.kind < NKINDS)
        base = kinds[t.kind].name;
    len = strlen(base);
    memcpy(buf, base, len);
    for (d = 0; d < t.dims && d < LC_MAX_DIMS; d++) {
        buf[len++] = '[';
        buf[len++] = ']';
    }
    buf[len] = '\0';
    return buf;
}

/* ------------------------------------------------------------------ */
/* Helpers                                                             */
/* ------------------------------------------------------------------ */

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

static int is_ident_char(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || is_digit(c) ||
           c == '_';
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

/* Length of `s` to quote: at most QUOTE_MAX bytes, cut at a UTF-8
 * character boundary. Use with "'%.*s%s'" and QUOTE(). */
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

static int fail(lc_str *err, const char *fmt, ...) LC_PRINTF(2, 3);

/* Sets the message and returns LC_ERR_PARSE, or LC_ERR_NOMEM. */
static int fail(lc_str *err, const char *fmt, ...)
{
    va_list ap;

    lc_str_clear(err);
    va_start(ap, fmt);
    lc_str_vprintf(err, fmt, ap);
    va_end(ap);
    return err->failed ? LC_ERR_NOMEM : LC_ERR_PARSE;
}

/*
 * Names the generated C and C++ code can't take as parameter or function
 * names: keywords of either language (up to C23 and C++23), and names the
 * harness and LeetCode's preludes already use.
 */
static const char *const reserved[] = {
    "EOF", "ListNode", "NULL", "Node", "Solution", "TreeNode", "alignas",
    "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool",
    "break", "case", "catch", "char", "char16_t", "char32_t", "char8_t",
    "class", "co_await", "co_return", "co_yield", "compl", "concept",
    "const", "const_cast", "consteval", "constexpr", "constinit",
    "continue", "decltype", "default", "delete", "do", "double",
    "dynamic_cast", "else", "enum", "errno", "explicit", "export", "extern",
    "false", "float", "for", "friend", "goto", "if", "inline", "int",
    "long", "main", "mutable", "namespace", "new", "noexcept", "not",
    "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
    "public", "register", "reinterpret_cast", "requires", "restrict",
    "return", "returnColumnSizes", "returnSize", "short", "signed",
    "sizeof", "static", "static_assert", "static_cast", "stderr", "stdin",
    "stdout", "struct", "switch",
    "template", "this", "thread_local", "throw", "true", "try", "typedef",
    "typeid", "typename", "typeof", "typeof_unqual", "union", "unsigned",
    "using", "virtual", "void", "volatile", "wchar_t", "while", "xor",
    "xor_eq",
};

static int is_reserved(span name)
{
    size_t i;

    if (name.n > 0 && name.p[0] == '_')
        return 1;
    if (name.n >= 3 && (memcmp(name.p, "lc_", 3) == 0 ||
                        memcmp(name.p, "LC_", 3) == 0))
        return 1;
    for (i = 0; i < sizeof reserved / sizeof reserved[0]; i++)
        if (span_eq(name, reserved[i]))
            return 1;
    return 0;
}

/*
 * Splits "type name" into its parts: the name is the identifier at the
 * end (not starting with a digit), the type is the trimmed rest. Either
 * may come back empty.
 */
static void split_decl(span decl, span *type, span *name)
{
    size_t j = decl.n;

    while (j > 0 && is_ident_char((unsigned char)decl.p[j - 1]))
        j--;
    while (j < decl.n && is_digit((unsigned char)decl.p[j]))
        j++;
    *name = mkspan(decl.p + j, decl.n - j);
    *type = trim(mkspan(decl.p, j));
}

/* Checks a type for `what`; void only when allow_void. */
static int check_type(span ts, int allow_void, const char *what,
                      lc_type *out, lc_str *err)
{
    int r = lookup_type(ts, out);

    if (r == TYPE_UNKNOWN)
        return fail(err, "%s: unknown type '%.*s%s'", what, QUOTE(ts));
    if (r == TYPE_UNSUPPORTED)
        return fail(err, "%s: type '%.*s%s' is not supported (too many "
                    "'[]')", what, QUOTE(ts));
    if (out->kind == LC_T_VOID && !allow_void)
        return fail(err, "%s: only a return type can be void", what);
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Parameters and signatures                                           */
/* ------------------------------------------------------------------ */

/* The next parameter in list[*i..): split on commas outside <>, [] and
 * (), so a type like map<int, string> stays whole in messages. */
static span next_param(span list, size_t *i)
{
    size_t start = *i, k;
    int depth = 0;

    for (k = start; k < list.n; k++) {
        char c = list.p[k];
        if (c == '<' || c == '[' || c == '(')
            depth++;
        else if ((c == '>' || c == ']' || c == ')') && depth > 0)
            depth--;
        else if (c == ',' && depth == 0)
            break;
    }
    *i = k + 1; /* past the comma, or past the end */
    return trim(mkspan(list.p + start, k - start));
}

int lc_parse_params(lc_arena *a, const char *p, size_t n, const char *what,
                    lc_param **out, size_t *count, lc_str *err)
{
    span list = trim(mkspan(p, n));
    lc_param *params;
    size_t np = 0, i, k;

    lc_str_clear(err);
    *out = NULL;
    *count = 0;
    if (list.n > 0) {
        /* Count first: every comma at depth 0 adds one. */
        for (i = 0; i <= list.n; np++)
            next_param(list, &i);
    }
    params = lc_arena_array(a, np ? np : 1, sizeof *params);
    if (!params)
        return LC_ERR_NOMEM;

    for (i = 0, k = 0; k < np; k++) {
        span param = next_param(list, &i), ts, name;
        size_t j;
        int rc;

        if (param.n == 0)
            return fail(err, "%s: parameter %lu is empty", what,
                        (unsigned long)(k + 1));
        split_decl(param, &ts, &name);
        if (name.n == 0 || ts.n == 0)
            return fail(err, "%s: parameter '%.*s%s' has no name", what,
                        QUOTE(param));
        rc = check_type(ts, 0, what, &params[k].type, err);
        if (rc != LC_OK)
            return rc;
        if (is_reserved(name))
            return fail(err, "%s: '%.*s%s' can't be used as a name", what,
                        QUOTE(name));
        for (j = 0; j < k; j++)
            if (span_eq(name, params[j].name))
                return fail(err, "%s: duplicate parameter name '%.*s%s'",
                            what, QUOTE(name));
        params[k].name = lc_arena_strndup(a, name.p, name.n);
        if (!params[k].name)
            return LC_ERR_NOMEM;
    }
    *out = params;
    *count = np;
    return LC_OK;
}

/* 1 if `name` is `base` followed by `suffix`. */
static int is_named(span name, const char *base, const char *suffix)
{
    size_t nb = strlen(base), ns = strlen(suffix);

    return name.n == nb + ns && memcmp(name.p, base, nb) == 0 &&
           memcmp(name.p + nb, suffix, ns) == 0;
}

/*
 * The generated C code names the function and its parameters as written,
 * and an array parameter's size `<name>Size` (and a 2D one's row sizes
 * `<name>ColSize`), so none of these names may be another's.
 */
static int check_c_names(span fname, const lc_param *params, size_t np,
                         const char *what, lc_str *err)
{
    size_t i, k;

    for (i = 0; i < np; i++)
        if (span_eq(fname, params[i].name))
            return fail(err, "%s: '%.*s%s' is both the function's name and "
                        "a parameter's", what, QUOTE(fname));
    for (i = 0; i < np; i++) {
        if (params[i].type.dims == 0)
            continue;
        for (k = 0; k <= np; k++) {
            span other = k < np ? mkspan(params[k].name,
                                         strlen(params[k].name))
                                : fname;

            if (is_named(other, params[i].name, "Size"))
                return fail(err, "%s: '%.*s%s' can't be used as a name, as "
                            "lc's C code gives it to the size of '%s'",
                            what, QUOTE(other), params[i].name);
            if (params[i].type.dims == 2 &&
                is_named(other, params[i].name, "ColSize"))
                return fail(err, "%s: '%.*s%s' can't be used as a name, as "
                            "lc's C code gives it to the row sizes of '%s'",
                            what, QUOTE(other), params[i].name);
        }
    }
    return LC_OK;
}

int lc_parse_sig(lc_arena *a, const char *p, size_t n, const char *what,
                 lc_sig *out, lc_str *err)
{
    span s = mkspan(p, n), head, tail, ts, name;
    const char *open = memchr(p, '(', n), *close = NULL;
    lc_param *params;
    size_t i, np;
    int rc;

    lc_str_clear(err);
    memset(out, 0, sizeof *out);
    for (i = n; i > 0; i--) {
        if (p[i - 1] == ')') {
            close = p + i - 1;
            break;
        }
    }
    if (!open || !close || close < open)
        return fail(err, "%s has no parentheses", what);
    tail = trim(mkspan(close + 1, (size_t)(s.p + s.n - close - 1)));
    if (tail.n > 0)
        return fail(err, "%s: unexpected '%.*s%s' after ')'", what,
                    QUOTE(tail));

    head = trim(mkspan(p, (size_t)(open - p)));
    split_decl(head, &ts, &name);
    if (name.n == 0 || ts.n == 0)
        return fail(err, "%s: expected a return type and a function name "
                    "before '('", what);
    rc = check_type(ts, 1, what, &out->ret, err);
    if (rc != LC_OK)
        return rc;
    if (is_reserved(name))
        return fail(err, "%s: '%.*s%s' can't be used as a name", what,
                    QUOTE(name));

    rc = lc_parse_params(a, open + 1, (size_t)(close - open - 1), what,
                         &params, &np, err);
    if (rc != LC_OK)
        return rc;
    rc = check_c_names(name, params, np, what, err);
    if (rc != LC_OK)
        return rc;
    out->name = lc_arena_strndup(a, name.p, name.n);
    if (!out->name)
        return LC_ERR_NOMEM;
    out->params = params;
    out->nparams = np;
    return LC_OK;
}

lc_param *lc_copy_params(lc_arena *a, const lc_param *params, size_t n)
{
    lc_param *c = lc_arena_array(a, n ? n : 1, sizeof *c);
    size_t i;

    if (!c)
        return NULL;
    for (i = 0; i < n; i++) {
        c[i].type = params[i].type;
        c[i].name = lc_arena_strdup(a, params[i].name);
        if (!c[i].name)
            return NULL;
    }
    return c;
}

lc_sig *lc_copy_sig(lc_arena *a, const lc_sig *sig)
{
    lc_sig *c = lc_arena_zalloc(a, sizeof *c);

    if (!c)
        return NULL;
    c->ret = sig->ret;
    c->name = lc_arena_strdup(a, sig->name);
    c->params = lc_copy_params(a, sig->params, sig->nparams);
    c->nparams = sig->nparams;
    if (!c->name || !c->params)
        return NULL;
    return c;
}

/* ------------------------------------------------------------------ */
/* compare:                                                            */
/* ------------------------------------------------------------------ */

lc_compare lc_default_compare(const lc_type *result)
{
    lc_compare c;

    memset(&c, 0, sizeof c);
    c.mode = LC_CMP_EXACT;
    if (result && result->kind == LC_T_DOUBLE) {
        c.mode = LC_CMP_FLOAT;
        c.tolerance = 1e-5;
    }
    return c;
}

/* Splits off the first word of s (up to a space or tab). */
static span first_word(span s, span *rest)
{
    size_t i = 0;

    while (i < s.n && !is_blank_char((unsigned char)s.p[i]))
        i++;
    *rest = trim(mkspan(s.p + i, s.n - i));
    return mkspan(s.p, i);
}

static int parse_levels(span s, unsigned *levels, lc_str *err)
{
    size_t i = 0;

    *levels = 0;
    if (s.n == 0)
        return fail(err, "compare: unordered needs the levels where order "
                    "doesn't matter, e.g. 'unordered 0'");
    for (;;) {
        const char *comma = memchr(s.p + i, ',', s.n - i);
        span lv = trim(mkspan(s.p + i, (size_t)((comma ? comma : s.p + s.n)
                                                 - (s.p + i))));
        unsigned bit;

        if (lv.n != 1 || !is_digit((unsigned char)lv.p[0]) ||
            lv.p[0] - '0' >= LC_MAX_DIMS)
            return fail(err, "compare: invalid level '%.*s%s' (expected a "
                        "comma-separated list of levels from 0 to %d)",
                        QUOTE(lv), LC_MAX_DIMS - 1);
        bit = 1u << (lv.p[0] - '0');
        if (*levels & bit)
            return fail(err, "compare: level %c is listed twice", lv.p[0]);
        *levels |= bit;
        if (!comma)
            return LC_OK;
        i = (size_t)(comma - s.p) + 1;
    }
}

int lc_parse_compare(const char *p, size_t n, const lc_type *result,
                     lc_compare *out, lc_str *err)
{
    span s = trim(mkspan(p, n)), rest, mode;
    char tname[LC_TYPE_NAME_MAX];
    lc_compare c;
    int rc;

    lc_str_clear(err);
    memset(&c, 0, sizeof c);
    mode = first_word(s, &rest);
    if (span_eq(mode, "exact") || span_eq(mode, "checker")) {
        c.mode = mode.p[0] == 'e' ? LC_CMP_EXACT : LC_CMP_CHECKER;
        if (rest.n > 0)
            return fail(err, "compare: unexpected '%.*s%s' after '%.*s'",
                        QUOTE(rest), (int)mode.n, mode.p);
    } else if (span_eq(mode, "float")) {
        size_t len;
        int is_int;

        c.mode = LC_CMP_FLOAT;
        c.tolerance = 1e-5;
        if (rest.n > 0) {
            len = lc_scan_number(rest.p, rest.n, &is_int);
            if (len != rest.n || rest.p[0] == '-' ||
                lc_strtod(rest.p, rest.n, &c.tolerance) != 0 ||
                !(c.tolerance > 0))
                return fail(err, "compare: invalid tolerance '%.*s%s' "
                            "(expected a positive number such as 1e-5)",
                            QUOTE(rest));
        }
    } else if (span_eq(mode, "unordered")) {
        c.mode = LC_CMP_UNORDERED;
        rc = parse_levels(rest, &c.levels, err);
        if (rc != LC_OK)
            return rc;
    } else if (mode.n == 0) {
        return fail(err, "compare: empty value (expected exact, float, "
                    "unordered or checker)");
    } else {
        return fail(err, "compare: unknown mode '%.*s%s' (expected exact, "
                    "float, unordered or checker)", QUOTE(mode));
    }

    if (result) {
        lc_type_name(*result, tname);
        if (c.mode == LC_CMP_FLOAT && result->kind != LC_T_DOUBLE)
            return fail(err, "compare: float needs a result made of doubles, "
                        "not %s", tname);
        if (c.mode == LC_CMP_UNORDERED) {
            int k;
            for (k = LC_MAX_DIMS - 1; k >= 0; k--)
                if ((c.levels & (1u << k)) && k >= result->dims)
                    return fail(err, "compare: %s has no level %d (levels "
                                "count its array nestings from 0)", tname,
                                k);
        }
    }
    *out = c;
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* check:                                                              */
/* ------------------------------------------------------------------ */

static int is_node_kind(lc_kind k)
{
    return k == LC_T_LIST || k == LC_T_TREE || k == LC_T_NARY;
}

int lc_parse_check(const char *p, size_t n, const lc_sig *sig,
                   lc_result_check *out, lc_str *err)
{
    span s = trim(mkspan(p, n)), rest, mode, name, extra;
    char tname[LC_TYPE_NAME_MAX];
    const lc_param *pr = NULL;
    lc_result_check c;
    size_t i;

    lc_str_clear(err);
    memset(&c, 0, sizeof c);
    mode = first_word(s, &rest);
    if (mode.n == 0)
        return fail(err, "check: empty value (expected 'param NAME' or "
                    "'prefix NAME')");
    if (span_eq(mode, "param"))
        c.mode = LC_RESULT_PARAM;
    else if (span_eq(mode, "prefix"))
        c.mode = LC_RESULT_PREFIX;
    else
        return fail(err, "check: unknown mode '%.*s%s' (expected param or "
                    "prefix)", QUOTE(mode));
    name = first_word(rest, &extra);
    if (name.n == 0)
        return fail(err, "check: %.*s needs the name of a parameter, e.g. "
                    "'%.*s nums'", (int)mode.n, mode.p, (int)mode.n, mode.p);
    if (extra.n > 0)
        return fail(err, "check: unexpected '%.*s%s' after '%.*s%s'",
                    QUOTE(extra), QUOTE(name));
    if (!sig)
        return fail(err, "check: needs a signature");
    for (i = 0; i < sig->nparams; i++)
        if (span_eq(name, sig->params[i].name)) {
            pr = &sig->params[i];
            c.param = i;
            break;
        }
    if (!pr)
        return fail(err, "check: '%.*s%s' is not a parameter of the "
                    "signature", QUOTE(name));

    lc_type_name(sig->ret, tname);
    if (c.mode == LC_RESULT_PARAM && sig->ret.kind != LC_T_VOID)
        return fail(err, "check: param needs a function that returns void, "
                    "not %s", tname);
    if (c.mode == LC_RESULT_PREFIX &&
        (sig->ret.kind != LC_T_INT || sig->ret.dims != 0))
        return fail(err, "check: prefix needs a function that returns int "
                    "(the count), not %s", tname);
    lc_type_name(pr->type, tname);
    if (is_node_kind(pr->type.kind))
        return fail(err, "check: lists and trees can't be checked yet, but "
                    "'%s' is %s", pr->name, tname);
    if (c.mode == LC_RESULT_PARAM && pr->type.dims == 0)
        return fail(err, "check: param needs an array such as int[] or "
                    "char[][], but '%s' is %s", pr->name, tname);
    if (c.mode == LC_RESULT_PREFIX && pr->type.dims != 1)
        return fail(err, "check: prefix needs a one-dimensional array such "
                    "as int[], but '%s' is %s", pr->name, tname);
    *out = c;
    return LC_OK;
}

const lc_type *lc_sig_result_type(const lc_sig *sig,
                                  const lc_result_check *check)
{
    if (!sig)
        return NULL;
    if (check && check->mode != LC_RESULT_RETURN &&
        check->param < sig->nparams)
        return &sig->params[check->param].type;
    return &sig->ret;
}
