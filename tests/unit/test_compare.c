#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* Tests for compare.c: exact, float and unordered comparison. */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static lc_ctx *ctx;
static lc_pool *pool;

static const lc_value *val(const char *type, const char *text)
{
    lc_type t;
    const lc_value *v = NULL;

    CHECK_INT(lc_type_parse(type, strlen(type), &t), LC_OK);
    CHECK_INT(lc_value_parse(ctx, pool, &t, text, strlen(text), &v), LC_OK);
    if (!v)
        fprintf(stderr, "    %s '%s': %s\n", type, text, lc_last_error(ctx));
    return v;
}

static lc_compare mode(const char *spec)
{
    lc_str err = LC_STR_INIT;
    lc_compare c;

    memset(&c, 0, sizeof c);
    CHECK_INT(lc_parse_compare(spec, strlen(spec), NULL, &c, &err), LC_OK);
    lc_str_free(&err);
    return c;
}

/* expected and got, both of `type`, must (want = 1) or must not match. */
static void match_(const char *spec, const char *type, const char *expected,
                   const char *got, int want, int line)
{
    lc_compare c = mode(spec);
    const lc_value *e = val(type, expected), *g = val(type, got);
    int m = -1;

    if (!e || !g)
        return;
    CHECK_INT(lc_value_match(ctx, &c, e, g, &m), LC_OK);
    if (m != want)
        fprintf(stderr, "    line %d: %s, %s vs %s\n", line, spec, expected,
                got);
    CHECK_INT(m, want);
}
#define match(spec, type, e, g) match_(spec, type, e, g, 1, __LINE__)
#define differ(spec, type, e, g) match_(spec, type, e, g, 0, __LINE__)

static void test_exact(void)
{
    match("exact", "int", "5", "5");
    differ("exact", "int", "5", "-5");
    match("exact", "long", "-9223372036854775808", "-9223372036854775808");
    match("exact", "bool", "true", "true");
    differ("exact", "bool", "true", "false");
    match("exact", "string", "\"a b\"", "\"a b\"");
    differ("exact", "string", "\"ab\"", "\"abc\"");
    differ("exact", "string", "\"\"", "\"a\"");
    match("exact", "char", "\"x\"", "\"x\"");
    match("exact", "int[]", "[]", "[]");
    match("exact", "int[]", "[1,2,3]", "[1,2,3]");
    differ("exact", "int[]", "[1,2,3]", "[1,2]");
    differ("exact", "int[]", "[1,2]", "[1,2,3]");
    differ("exact", "int[]", "[1,2,3]", "[3,2,1]");
    match("exact", "int[][]", "[[1],[]]", "[[1],[]]");
    differ("exact", "int[][]", "[[1],[]]", "[[],[1]]");
    match("exact", "TreeNode", "[1,null,2]", "[1,null,2,null,null]");
    differ("exact", "TreeNode", "[1,null,2]", "[1,2]");
    differ("exact", "TreeNode", "[]", "[1]");
    match("exact", "double", "0.5", "0.5");
    match("exact", "double", "0", "-0.0");
    differ("exact", "double", "0.1", "0.10000000000000002");
}

static void test_float(void)
{
    match("float 1e-5", "double", "2.5", "2.500009");
    match("float 1e-5", "double", "2.5", "2.499991");
    differ("float 1e-5", "double", "2.5", "2.50002");
    differ("float 1e-5", "double", "-1", "1");
    match("float 0.5", "double", "1", "1.5");
    differ("float 0.5", "double", "1", "1.5000001");
    match("float 1e-5", "double[]", "[1,2]", "[1.000001,1.999999]");
    differ("float 1e-5", "double[]", "[1,2]", "[1.000001,2.1]");
    differ("float 1e-5", "double[]", "[1,2]", "[1]");
    match("float 1e-5", "double[][]", "[[0.5],[]]", "[[0.500001],[]]");
    /* The tolerance is absolute, even for large values. */
    differ("float 1e-5", "double", "1000000", "1000000.001");

    /* NaN from a program never matches. */
    {
        lc_compare c = mode("float 1e-5");
        const lc_value *e = val("double", "1");
        lc_value nan_v;
        int m = -1;

        memset(&nan_v, 0, sizeof nan_v);
        nan_v.kind = LC_V_FLOAT;
        nan_v.u.f = NAN;
        CHECK_INT(lc_value_match(ctx, &c, e, &nan_v, &m), LC_OK);
        CHECK_INT(m, 0);
        c = mode("exact");
        CHECK_INT(lc_value_match(ctx, &c, e, &nan_v, &m), LC_OK);
        CHECK_INT(m, 0);
    }
}

static void test_unordered(void)
{
    lc_str before = LC_STR_INIT, after = LC_STR_INIT;

    match("unordered 0", "int[]", "[1,2,3]", "[3,1,2]");
    match("unordered 0", "int[]", "[]", "[]");
    match("unordered 0", "int[]", "[1,1,2]", "[1,2,1]");
    differ("unordered 0", "int[]", "[1,1,2]", "[1,2,2]");
    differ("unordered 0", "int[]", "[1,2]", "[1,2,2]");
    differ("unordered 0", "int[]", "[-1,2]", "[1,2]");
    match("unordered 0", "string[]", "[\"b\",\"a\",\"ab\"]",
          "[\"ab\",\"b\",\"a\"]");
    differ("unordered 0", "string[]", "[\"a\",\"ab\"]", "[\"a\",\"ba\"]");

    /* Level 1 only: order inside each list doesn't matter, but the order
     * of the lists does (problem 2215). */
    match("unordered 1", "int[][]", "[[1,2],[3]]", "[[2,1],[3]]");
    differ("unordered 1", "int[][]", "[[1,2],[3]]", "[[3],[2,1]]");
    /* Level 0 only: lists in any order, but each list as given. */
    match("unordered 0", "int[][]", "[[1,2],[3]]", "[[3],[1,2]]");
    differ("unordered 0", "int[][]", "[[1,2],[3]]", "[[3],[2,1]]");
    match("unordered 0,1", "int[][]", "[[1,2],[3]]", "[[3],[2,1]]");
    match("unordered 0,1", "string[][]", "[[\"eat\",\"tea\"],[\"bat\"]]",
          "[[\"bat\"],[\"tea\",\"eat\"]]");
    differ("unordered 0,1", "string[][]", "[[\"eat\",\"tea\"],[\"bat\"]]",
           "[[\"bat\",\"tea\"],[\"eat\"]]");
    /* Sorting inner lists first makes equal multisets compare equal. */
    match("unordered 0,1", "int[][]", "[[2,1],[1,3],[1,2]]",
          "[[3,1],[2,1],[2,1]]");

    /* Trees are compared whole; only the order of the list changes. */
    match("unordered 0", "TreeNode[]", "[[1,null,2],[2,1]]",
          "[[2,1],[1,null,2]]");
    differ("unordered 0", "TreeNode[]", "[[1,null,2]]", "[[1,2]]");
    match("unordered 0", "double[]", "[0.5,0.25]", "[0.25,0.5]");
    match("unordered 0", "bool[]", "[true,false,true]", "[true,true,false]");

    /* The values themselves are left as they were. */
    {
        lc_compare c = mode("unordered 0,1");
        const lc_value *e = val("int[][]", "[[3,2],[1]]");
        const lc_value *g = val("int[][]", "[[1],[2,3]]");
        int m = 0;

        lc_value_print(e, &before);
        CHECK_INT(lc_value_match(ctx, &c, e, g, &m), LC_OK);
        CHECK_INT(m, 1);
        lc_value_print(e, &after);
        CHECK_STR(lc_str_cstr(&after), lc_str_cstr(&before));
    }

    /* A long list in reverse order. */
    {
        lc_str a = LC_STR_INIT, b = LC_STR_INIT;
        lc_compare c = mode("unordered 0");
        const lc_value *e, *g;
        int i, m = 0;

        lc_str_putc(&a, '[');
        lc_str_putc(&b, '[');
        for (i = 0; i < 100000; i++) {
            lc_str_printf(&a, "%s%d", i ? "," : "", i % 977);
            lc_str_printf(&b, "%s%d", i ? "," : "", (99999 - i) % 977);
        }
        lc_str_putc(&a, ']');
        lc_str_putc(&b, ']');
        e = val("int[]", a.data);
        g = val("int[]", b.data);
        if (e && g) {
            CHECK_INT(lc_value_match(ctx, &c, e, g, &m), LC_OK);
            CHECK_INT(m, 1);
        }
        lc_str_free(&a);
        lc_str_free(&b);
    }
    lc_str_free(&before);
    lc_str_free(&after);
}

static void test_checker_and_errors(void)
{
    lc_compare c = mode("checker");
    const lc_value *v = val("int", "1");
    int m = 7;

    CHECK_INT(lc_value_match(ctx, &c, v, v, &m), LC_ERR_INVALID);
    CHECK_STR(lc_last_error(ctx), "compare: checker needs the pack's "
              "checker, which lc_value_match can't run");
    CHECK_INT(m, 0);
    CHECK_INT(lc_value_match(ctx, NULL, v, v, &m), LC_ERR_INVALID);
    CHECK_INT(lc_value_match(ctx, &c, NULL, v, &m), LC_ERR_INVALID);
    c.mode = (lc_cmp_mode)99;
    CHECK_INT(lc_value_match(ctx, &c, v, v, &m), LC_ERR_INVALID);
}

int main(void)
{
    ctx = lc_open(NULL);
    pool = lc_pool_new();
    if (!ctx || !pool)
        return 1;
    RUN(test_exact);
    RUN(test_float);
    RUN(test_unordered);
    RUN(test_checker_and_errors);
    lc_pool_free(pool);
    lc_close(ctx);
    return test_report();
}
