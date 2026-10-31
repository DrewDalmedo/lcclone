#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* Tests for types.c: the type table, signatures, parameters, compare. */

#include <stdio.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static void check_parse(const char *s, lc_kind kind, int dims)
{
    lc_type t;
    char name[LC_TYPE_NAME_MAX];

    CHECK_INT(lc_type_parse(s, strlen(s), &t), LC_OK);
    CHECK_INT(t.kind, kind);
    CHECK_INT(t.dims, dims);
    CHECK_STR(lc_type_name(t, name), s);
}

static void check_bad_type(const char *s)
{
    lc_type t = {LC_T_INT, 1};

    CHECK_INT(lc_type_parse(s, strlen(s), &t), LC_ERR_PARSE);
    /* *out is left alone on failure. */
    CHECK_INT(t.kind, LC_T_INT);
    CHECK_INT(t.dims, 1);
}

static void test_type_table(void)
{
    static const char *const scalars[] = {
        "int", "long", "uint32", "double", "bool", "char", "string"
    };
    static const lc_kind skinds[] = {
        LC_T_INT, LC_T_LONG, LC_T_UINT32, LC_T_DOUBLE, LC_T_BOOL, LC_T_CHAR,
        LC_T_STRING
    };
    char buf[32];
    size_t i;

    for (i = 0; i < sizeof scalars / sizeof scalars[0]; i++) {
        check_parse(scalars[i], skinds[i], 0);
        snprintf(buf, sizeof buf, "%s[]", scalars[i]);
        check_parse(buf, skinds[i], 1);
        snprintf(buf, sizeof buf, "%s[][]", scalars[i]);
        check_parse(buf, skinds[i], 2);
        snprintf(buf, sizeof buf, "%s[][][]", scalars[i]);
        check_bad_type(buf);
    }
    check_parse("void", LC_T_VOID, 0);
    check_parse("ListNode", LC_T_LIST, 0);
    check_parse("ListNode[]", LC_T_LIST, 1);
    check_parse("TreeNode", LC_T_TREE, 0);
    check_parse("TreeNode[]", LC_T_TREE, 1);
    check_parse("NaryNode", LC_T_NARY, 0);
    check_parse("NaryNode[]", LC_T_NARY, 1);

    check_bad_type("void[]");
    check_bad_type("ListNode[][]");
    check_bad_type("TreeNode[][]");
    check_bad_type("");
    check_bad_type("[]");
    check_bad_type("Int");
    check_bad_type("int ");
    check_bad_type(" int");
    check_bad_type("int[ ]");
    check_bad_type("int[");
    check_bad_type("int]");
    check_bad_type("vector<int>");
    check_bad_type("Node");
    check_bad_type("int*");
    /* A length, not the NUL, ends the name. */
    {
        lc_type t;
        CHECK_INT(lc_type_parse("int[]x", 5, &t), LC_OK);
        CHECK_INT(t.dims, 1);
    }
    CHECK_INT(lc_type_parse(NULL, 0, NULL), LC_ERR_INVALID);

    /* Every name fits in LC_TYPE_NAME_MAX. */
    {
        lc_type t;
        int k, d;
        for (k = LC_T_VOID; k <= LC_T_NARY; k++) {
            for (d = 0; d <= LC_MAX_DIMS; d++) {
                t.kind = (lc_kind)k;
                t.dims = d;
                CHECK(strlen(lc_type_name(t, buf)) < LC_TYPE_NAME_MAX);
            }
        }
    }
}

/* Parses a signature; NULL want_err means it must succeed. */
static void sig_case(const char *text, const char *want_err, lc_sig *out,
                     lc_arena *a)
{
    lc_str err = LC_STR_INIT;
    int rc = lc_parse_sig(a, text, strlen(text), "signature", out, &err);

    if (want_err) {
        CHECK_INT(rc, LC_ERR_PARSE);
        CHECK_STR(lc_str_cstr(&err), want_err);
    } else {
        CHECK_INT(rc, LC_OK);
        if (rc != LC_OK)
            fprintf(stderr, "    error: %s\n", lc_str_cstr(&err));
    }
    lc_str_free(&err);
}

static void test_signatures(void)
{
    lc_arena a = {0};
    lc_sig s;

    sig_case("int[] twoSum(int[] nums, int target)", NULL, &s, &a);
    CHECK_INT(s.ret.kind, LC_T_INT);
    CHECK_INT(s.ret.dims, 1);
    CHECK_STR(s.name, "twoSum");
    CHECK_INT(s.nparams, 2);

    sig_case("  void   merge ( int[] nums1 ,int m,\tint[] nums2, int n )  ",
             NULL, &s, &a);
    CHECK_INT(s.ret.kind, LC_T_VOID);
    CHECK_STR(s.name, "merge");
    CHECK_INT(s.nparams, 4);
    if (s.nparams == 4) {
        CHECK_STR(s.params[0].name, "nums1");
        CHECK_STR(s.params[1].name, "m");
        CHECK_STR(s.params[3].name, "n");
        CHECK_INT(s.params[2].type.dims, 1);
    }

    sig_case("string helloWorld()", NULL, &s, &a);
    CHECK_INT(s.nparams, 0);
    CHECK(s.params != NULL);
    sig_case("char[][] f(TreeNode root, NaryNode[] kids, uint32 n, long k, "
             "bool b, double d, char c, string[][] grid)", NULL, &s, &a);
    CHECK_INT(s.nparams, 8);
    CHECK_INT(s.ret.kind, LC_T_CHAR);
    CHECK_INT(s.ret.dims, 2);

    sig_case("int f", "signature has no parentheses", &s, &a);
    sig_case("int f)(", "signature has no parentheses", &s, &a);
    sig_case("int f(int x) {", "signature: unexpected '{' after ')'", &s, &a);
    sig_case("int f(int x);", "signature: unexpected ';' after ')'", &s, &a);
    sig_case("(int x)", "signature: expected a return type and a function "
             "name before '('", &s, &a);
    sig_case("int 9f(int x)", "signature: unknown type 'int 9'", &s, &a);
    sig_case("Int f(int x)", "signature: unknown type 'Int'", &s, &a);
    sig_case("int[][][] f(int x)", "signature: type 'int[][][]' is not "
             "supported (too many '[]')", &s, &a);
    sig_case("int if(int x)", "signature: 'if' can't be used as a name", &s,
             &a);
    sig_case("int f(int x, int y, int x)", "signature: duplicate parameter "
             "name 'x'", &s, &a);
    sig_case("int f(int Solution)", "signature: 'Solution' can't be used as "
             "a name", &s, &a);
    sig_case("int f(string s, string[] nullptr)", "signature: 'nullptr' "
             "can't be used as a name", &s, &a);
    sig_case("int f(int x, )", "signature: parameter 2 is empty", &s, &a);

    /* Names the generated C code would use twice. */
    sig_case("void nums(int[] nums)", "signature: 'nums' is both the "
             "function's name and a parameter's", &s, &a);
    sig_case("int f(int[] nums, int numsSize)", "signature: 'numsSize' "
             "can't be used as a name, as lc's C code gives it to the size "
             "of 'nums'", &s, &a);
    sig_case("int f(int numsSize, string[] nums)", "signature: 'numsSize' "
             "can't be used as a name, as lc's C code gives it to the size "
             "of 'nums'", &s, &a);
    sig_case("int numsSize(int[] nums)", "signature: 'numsSize' can't be "
             "used as a name, as lc's C code gives it to the size of 'nums'",
             &s, &a);
    sig_case("int f(char[][] g, int gColSize)", "signature: 'gColSize' "
             "can't be used as a name, as lc's C code gives it to the row "
             "sizes of 'g'", &s, &a);
    sig_case("int[] f(int returnSize)", "signature: 'returnSize' can't be "
             "used as a name", &s, &a);
    sig_case("int f(int returnColumnSizes)", "signature: "
             "'returnColumnSizes' can't be used as a name", &s, &a);
    /* Only arrays have sizes, and only 2D ones row sizes. */
    sig_case("int f(int n, int nSize, int[] g, int gColSize)", NULL, &s,
             &a);

    lc_arena_free(&a);
}

static void test_params(void)
{
    lc_arena a = {0};
    lc_str err = LC_STR_INIT;
    lc_param *ps;
    size_t n;

    CHECK_INT(lc_parse_params(&a, " \t", 2, "hidden", &ps, &n, &err), LC_OK);
    CHECK_INT(n, 0);
    CHECK(ps != NULL);
    CHECK_INT(lc_parse_params(&a, "int skipA, int skipB", 20, "hidden", &ps,
                              &n, &err), LC_OK);
    CHECK_INT(n, 2);
    if (n == 2) {
        CHECK_STR(ps[0].name, "skipA");
        CHECK_STR(ps[1].name, "skipB");
    }
    CHECK_INT(lc_parse_params(&a, "int", 3, "hidden", &ps, &n, &err),
              LC_ERR_PARSE);
    CHECK_STR(lc_str_cstr(&err), "hidden: parameter 'int' has no name");
    CHECK(ps == NULL);
    CHECK_INT(n, 0);

    /* Long values are cut at 60 bytes in messages. */
    {
        char longp[200];
        memset(longp, 'q', sizeof longp);
        memcpy(longp + 150, " name", 5);
        CHECK_INT(lc_parse_params(&a, longp, 155, "hidden", &ps, &n, &err),
                  LC_ERR_PARSE);
        CHECK(strstr(lc_str_cstr(&err), "...'") != NULL);
        CHECK(strlen(lc_str_cstr(&err)) < 120);
    }

    lc_str_free(&err);
    lc_arena_free(&a);
}

static void cmp_ok(const char *text, const char *result, lc_cmp_mode mode,
                   double tol, unsigned levels)
{
    lc_str err = LC_STR_INIT;
    lc_compare c;
    lc_type t;
    int rc;

    if (result)
        CHECK_INT(lc_type_parse(result, strlen(result), &t), LC_OK);
    rc = lc_parse_compare(text, strlen(text), result ? &t : NULL, &c, &err);
    CHECK_INT(rc, LC_OK);
    if (rc != LC_OK) {
        fprintf(stderr, "    error: %s\n", lc_str_cstr(&err));
    } else {
        CHECK_INT(c.mode, mode);
        CHECK(c.tolerance == tol);
        CHECK_INT(c.levels, levels);
    }
    lc_str_free(&err);
}

static void cmp_bad(const char *text, const char *result, const char *msg)
{
    lc_str err = LC_STR_INIT;
    lc_compare c;
    lc_type t;

    memset(&c, 0x5A, sizeof c);
    if (result)
        CHECK_INT(lc_type_parse(result, strlen(result), &t), LC_OK);
    CHECK_INT(lc_parse_compare(text, strlen(text), result ? &t : NULL, &c,
                               &err), LC_ERR_PARSE);
    CHECK_STR(lc_str_cstr(&err), msg);
    CHECK_INT(c.mode, 0x5A5A5A5A); /* untouched */
    lc_str_free(&err);
}

static void test_compare_field(void)
{
    lc_compare c;
    lc_type dbl = {LC_T_DOUBLE, 2}, str = {LC_T_STRING, 0};

    cmp_ok("exact", NULL, LC_CMP_EXACT, 0, 0);
    cmp_ok("  exact\t", "int[]", LC_CMP_EXACT, 0, 0);
    cmp_ok("checker", "TreeNode", LC_CMP_CHECKER, 0, 0);
    cmp_ok("float", "double", LC_CMP_FLOAT, 1e-5, 0);
    cmp_ok("float 1e-6", "double[]", LC_CMP_FLOAT, 1e-6, 0);
    cmp_ok("float   0.25", "double[][]", LC_CMP_FLOAT, 0.25, 0);
    cmp_ok("float 2", NULL, LC_CMP_FLOAT, 2, 0);
    cmp_ok("unordered 0", "int[]", LC_CMP_UNORDERED, 0, 1);
    cmp_ok("unordered 1", "int[][]", LC_CMP_UNORDERED, 0, 2);
    cmp_ok("unordered 0,1", "string[][]", LC_CMP_UNORDERED, 0, 3);
    cmp_ok("unordered 1 , 0", "int[][]", LC_CMP_UNORDERED, 0, 3);
    cmp_ok("unordered 0", "TreeNode[]", LC_CMP_UNORDERED, 0, 1);
    cmp_ok("unordered 1", NULL, LC_CMP_UNORDERED, 0, 2);

    cmp_bad("", NULL, "compare: empty value (expected exact, float, "
            "unordered or checker)");
    cmp_bad("Exact", NULL, "compare: unknown mode 'Exact' (expected exact, "
            "float, unordered or checker)");
    cmp_bad("exact 1", NULL, "compare: unexpected '1' after 'exact'");
    cmp_bad("checker x.c", NULL, "compare: unexpected 'x.c' after 'checker'");
    cmp_bad("float 0", NULL, "compare: invalid tolerance '0' (expected a "
            "positive number such as 1e-5)");
    cmp_bad("float -1", NULL, "compare: invalid tolerance '-1' (expected a "
            "positive number such as 1e-5)");
    cmp_bad("float 1e-5 abs", NULL, "compare: invalid tolerance '1e-5 abs' "
            "(expected a positive number such as 1e-5)");
    cmp_bad("float .5", NULL, "compare: invalid tolerance '.5' (expected a "
            "positive number such as 1e-5)");
    cmp_bad("float 1e999", NULL, "compare: invalid tolerance '1e999' "
            "(expected a positive number such as 1e-5)");
    cmp_bad("float", "int", "compare: float needs a result made of doubles, "
            "not int");
    cmp_bad("float", "TreeNode", "compare: float needs a result made of "
            "doubles, not TreeNode");
    cmp_bad("unordered", NULL, "compare: unordered needs the levels where "
            "order doesn't matter, e.g. 'unordered 0'");
    cmp_bad("unordered 2", NULL, "compare: invalid level '2' (expected a "
            "comma-separated list of levels from 0 to 1)");
    cmp_bad("unordered 0,", NULL, "compare: invalid level '' (expected a "
            "comma-separated list of levels from 0 to 1)");
    cmp_bad("unordered 01", NULL, "compare: invalid level '01' (expected a "
            "comma-separated list of levels from 0 to 1)");
    cmp_bad("unordered 0 1", NULL, "compare: invalid level '0 1' (expected a "
            "comma-separated list of levels from 0 to 1)");
    cmp_bad("unordered 1,1", NULL, "compare: level 1 is listed twice");
    cmp_bad("unordered 0", "int", "compare: int has no level 0 (levels count "
            "its array nestings from 0)");
    cmp_bad("unordered 0", "ListNode", "compare: ListNode has no level 0 "
            "(levels count its array nestings from 0)");
    cmp_bad("unordered 0,1", "int[]", "compare: int[] has no level 1 (levels "
            "count its array nestings from 0)");

    c = lc_default_compare(NULL);
    CHECK_INT(c.mode, LC_CMP_EXACT);
    c = lc_default_compare(&str);
    CHECK_INT(c.mode, LC_CMP_EXACT);
    c = lc_default_compare(&dbl);
    CHECK_INT(c.mode, LC_CMP_FLOAT);
    CHECK(c.tolerance == 1e-5);
}

/* lc_parse_check against the signature `sig`. */
static void check_ok(const char *text, const char *sig, lc_result_mode mode,
                     size_t param, const char *type)
{
    lc_arena a = {0};
    lc_str err = LC_STR_INIT;
    lc_result_check c;
    lc_sig s;
    char name[LC_TYPE_NAME_MAX];
    int rc;

    CHECK_INT(lc_parse_sig(&a, sig, strlen(sig), "signature", &s, &err),
              LC_OK);
    memset(&c, 0x5A, sizeof c);
    rc = lc_parse_check(text, strlen(text), &s, &c, &err);
    CHECK_INT(rc, LC_OK);
    if (rc != LC_OK) {
        fprintf(stderr, "    error: %s\n", lc_str_cstr(&err));
    } else {
        CHECK_INT(c.mode, mode);
        CHECK_INT(c.param, param);
        CHECK_STR(lc_type_name(*lc_sig_result_type(&s, &c), name), type);
    }
    lc_str_free(&err);
    lc_arena_free(&a);
}

static void check_bad(const char *text, const char *sig, const char *msg)
{
    lc_arena a = {0};
    lc_str err = LC_STR_INIT;
    lc_result_check c;
    lc_sig s;

    if (sig)
        CHECK_INT(lc_parse_sig(&a, sig, strlen(sig), "signature", &s, &err),
                  LC_OK);
    memset(&c, 0x5A, sizeof c);
    CHECK_INT(lc_parse_check(text, strlen(text), sig ? &s : NULL, &c, &err),
              LC_ERR_PARSE);
    CHECK_STR(lc_str_cstr(&err), msg);
    CHECK_INT(c.mode, 0x5A5A5A5A); /* untouched */
    lc_str_free(&err);
    lc_arena_free(&a);
}

static void test_check_field(void)
{
    lc_arena a = {0};
    lc_str err = LC_STR_INIT;
    lc_result_check none;
    lc_sig s;

    check_ok("param nums", "void f(int[] nums)", LC_RESULT_PARAM, 0,
             "int[]");
    check_ok("  param\t a ", "void merge(int[] a, int m, int[] b, int n)",
             LC_RESULT_PARAM, 0, "int[]");
    check_ok("param b", "void merge(int[] a, int m, int[] b, int n)",
             LC_RESULT_PARAM, 2, "int[]");
    check_ok("param g", "void f(int k, char[][] g)", LC_RESULT_PARAM, 1,
             "char[][]");
    check_ok("param s", "void f(string[] s)", LC_RESULT_PARAM, 0,
             "string[]");
    check_ok("prefix nums", "int f(int[] nums, int val)", LC_RESULT_PREFIX,
             0, "int[]");
    check_ok("prefix xs", "int f(int k, double[] xs)", LC_RESULT_PREFIX, 1,
             "double[]");

    check_bad("", "void f(int[] a)", "check: empty value (expected 'param "
              "NAME' or 'prefix NAME')");
    check_bad("result a", "void f(int[] a)", "check: unknown mode 'result' "
              "(expected param or prefix)");
    check_bad("Param a", "void f(int[] a)", "check: unknown mode 'Param' "
              "(expected param or prefix)");
    check_bad("param", "void f(int[] a)", "check: param needs the name of a "
              "parameter, e.g. 'param nums'");
    check_bad("prefix  ", "int f(int[] a)", "check: prefix needs the name of "
              "a parameter, e.g. 'prefix nums'");
    check_bad("param a b", "void f(int[] a, int[] b)", "check: unexpected "
              "'b' after 'a'");
    check_bad("param a", NULL, "check: needs a signature");
    check_bad("param x", "void f(int[] a)", "check: 'x' is not a parameter "
              "of the signature");
    check_bad("param f", "void f(int[] a)", "check: 'f' is not a parameter "
              "of the signature");
    check_bad("param a", "int f(int[] a)", "check: param needs a function "
              "that returns void, not int");
    check_bad("param a", "int[] f(int[] a)", "check: param needs a function "
              "that returns void, not int[]");
    check_bad("prefix a", "void f(int[] a)", "check: prefix needs a function "
              "that returns int (the count), not void");
    check_bad("prefix a", "long f(int[] a)", "check: prefix needs a function "
              "that returns int (the count), not long");
    check_bad("prefix a", "int[] f(int[] a)", "check: prefix needs a "
              "function that returns int (the count), not int[]");
    check_bad("param n", "void f(int n)", "check: param needs an array such "
              "as int[] or char[][], but 'n' is int");
    check_bad("param s", "void f(string s)", "check: param needs an array "
              "such as int[] or char[][], but 's' is string");
    check_bad("param h", "void f(ListNode h)", "check: lists and trees can't "
              "be checked yet, but 'h' is ListNode");
    check_bad("param t", "void f(TreeNode[] t)", "check: lists and trees "
              "can't be checked yet, but 't' is TreeNode[]");
    check_bad("prefix g", "int f(int[][] g)", "check: prefix needs a "
              "one-dimensional array such as int[], but 'g' is int[][]");
    check_bad("prefix k", "int f(int k)", "check: prefix needs a "
              "one-dimensional array such as int[], but 'k' is int");
    check_bad("prefix l", "int f(ListNode[] l)", "check: lists and trees "
              "can't be checked yet, but 'l' is ListNode[]");

    /* Without check:, the result is what the function returns. */
    CHECK_INT(lc_parse_sig(&a, "long f(int[] a)", 15, "signature", &s, &err),
              LC_OK);
    memset(&none, 0, sizeof none);
    CHECK(lc_sig_result_type(&s, &none) == &s.ret);
    CHECK(lc_sig_result_type(&s, NULL) == &s.ret);
    CHECK(lc_sig_result_type(NULL, &none) == NULL);
    lc_str_free(&err);
    lc_arena_free(&a);
}

static void test_copy(void)
{
    lc_arena a = {0}, b = {0};
    lc_str err = LC_STR_INIT;
    lc_sig s, *c;

    CHECK_INT(lc_parse_sig(&a, "long f(int[] xs, string s)", 26, "signature",
                           &s, &err), LC_OK);
    c = lc_copy_sig(&b, &s);
    lc_arena_free(&a);
    CHECK(c != NULL);
    if (c) {
        CHECK_STR(c->name, "f");
        CHECK_INT(c->ret.kind, LC_T_LONG);
        CHECK_INT(c->nparams, 2);
        if (c->nparams == 2) {
            CHECK_STR(c->params[0].name, "xs");
            CHECK_STR(c->params[1].name, "s");
            CHECK_INT(c->params[1].type.kind, LC_T_STRING);
        }
    }
    CHECK(lc_copy_params(&b, NULL, 0) != NULL);
    lc_str_free(&err);
    lc_arena_free(&b);
}

int main(void)
{
    RUN(test_type_table);
    RUN(test_signatures);
    RUN(test_params);
    RUN(test_compare_field);
    RUN(test_check_field);
    RUN(test_copy);
    return test_report();
}
