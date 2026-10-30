#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* Tests for wire.c: encoding values for the harness and decoding results. */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static lc_ctx *ctx;
static lc_pool *pool;

static lc_type ty(const char *name)
{
    lc_type t = {LC_T_VOID, 0};

    CHECK_INT(lc_type_parse(name, strlen(name), &t), LC_OK);
    return t;
}

/* The literal `text` of `type` must encode as `wire` and decode back to
 * the same printed value. */
static void enc_(const char *type, const char *text, const char *wire,
                 int line)
{
    lc_type t = ty(type);
    lc_str out = LC_STR_INIT, a = LC_STR_INIT, b = LC_STR_INIT;
    const lc_value *v = NULL, *d = NULL;
    size_t pos = 0;

    CHECK_INT(lc_value_parse(ctx, pool, &t, text, strlen(text), &v), LC_OK);
    if (!v) {
        fprintf(stderr, "    line %d: %s\n", line, lc_last_error(ctx));
        return;
    }
    CHECK_INT(lc_wire_encode(ctx, &t, v, &out), LC_OK);
    if (strcmp(lc_str_cstr(&out), wire) != 0)
        fprintf(stderr, "    line %d\n", line);
    CHECK_STR(lc_str_cstr(&out), wire);
    CHECK_INT(lc_wire_decode(ctx, pool, &t, out.data, out.len, &pos, &d),
              LC_OK);
    CHECK_INT(pos, out.len);
    if (d) {
        lc_value_print(v, &a);
        lc_value_print(d, &b);
        CHECK_STR(lc_str_cstr(&b), lc_str_cstr(&a));
    }
    lc_str_free(&out);
    lc_str_free(&a);
    lc_str_free(&b);
}
#define enc(type, text, wire) enc_(type, text, wire, __LINE__)

static void test_encode(void)
{
    /* The table in docs/PLAN.md section 5.3. */
    enc("int", "-5", "-5");
    enc("bool", "true", "1");
    enc("string", "\"a b\"", "3:a b");
    enc("int[]", "[2,7,11]", "3 2 7 11");
    enc("int[][]", "[[1,2],[3]]", "2 2 1 2 1 3");
    enc("string[]", "[\"ab\",\"c\"]", "2 2:ab 1:c");
    enc("TreeNode", "[1,null,2]", "3 1 n 2");

    enc("int", "2147483647", "2147483647");
    enc("long", "-9223372036854775808", "-9223372036854775808");
    enc("uint32", "4294967295", "4294967295");
    enc("bool", "false", "0");
    enc("double", "2.5", "2.5");
    enc("double", "2", "2");
    enc("double", "-0.0", "-0");
    enc("double", "0.1", "0.1");
    enc("double", "1e300", "1e+300");
    enc("double", "0.30000000000000004", "0.30000000000000004");
    enc("char", "\"a\"", "97");
    enc("char", "\" \"", "32");
    enc("char[]", "[\"h\",\"i\"]", "2 104 105");
    enc("string", "\"\"", "0:");
    enc("string", "\"line\\nnext\"", "9:line\nnext");
    enc("string", "\"caf\\u00e9\"", "5:caf\xc3\xa9");
    enc("string[]", "[\"\",\"\"]", "2 0: 0:");
    enc("int[]", "[]", "0");
    enc("int[][]", "[[],[]]", "2 0 0");
    enc("bool[]", "[true,false]", "2 1 0");
    enc("double[]", "[0.5,-1]", "2 0.5 -1");
    enc("ListNode", "[1,2,3]", "3 1 2 3");
    enc("ListNode", "[]", "0");
    enc("ListNode[]", "[[1],[]]", "2 1 1 0");
    enc("TreeNode", "[]", "0");
    enc("TreeNode", "[5,3,8,1,null,null,9,null,2]", "9 5 3 8 1 n n 9 n 2");
    enc("TreeNode", "[1,null,2,null,null]", "3 1 n 2");
    enc("TreeNode[]", "[[1],[]]", "2 1 1 0");
    enc("NaryNode", "[1,null,3,2,4,null,5,6]", "8 1 n 3 2 4 n 5 6");
    enc("string[][]", "[[\"a b\"],[]]", "2 1 3:a b 0");
}

static void test_encode_errors(void)
{
    lc_str out = LC_STR_INIT;
    lc_type t = ty("int");
    lc_value v, items[2];

    memset(&v, 0, sizeof v);
    v.kind = LC_V_INT;
    v.u.i = 2147483648LL;
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_ERR_INVALID);
    CHECK_STR(lc_last_error(ctx), "lc_wire_encode: the value doesn't fit "
              "int");
    t = ty("uint32");
    v.u.i = -1;
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_ERR_INVALID);
    t = ty("string");
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_ERR_INVALID);
    t = ty("double");
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_ERR_INVALID);

    memset(items, 0, sizeof items);
    items[0].kind = LC_V_INT;
    items[1].kind = LC_V_NULL;
    v.kind = LC_V_ARRAY;
    v.len = 2;
    v.u.items = items;
    t = ty("ListNode");
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_ERR_INVALID);
    t = ty("TreeNode");
    lc_str_clear(&out);
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_OK);
    CHECK_STR(lc_str_cstr(&out), "2 0 n");
    t = ty("int[][]");
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_ERR_INVALID);

    t = ty("void");
    lc_str_clear(&out);
    CHECK_INT(lc_wire_encode(ctx, &t, NULL, &out), LC_OK);
    CHECK_STR(lc_str_cstr(&out), "");
    t = ty("int");
    CHECK_INT(lc_wire_encode(ctx, &t, NULL, &out), LC_ERR_INVALID);
    t.dims = 3;
    CHECK_INT(lc_wire_encode(ctx, &t, &v, &out), LC_ERR_INVALID);
    lc_str_free(&out);
}

/* Decodes `wire` as `type`; must give the literal `printed`. */
static void dec_(const char *type, const char *wire, size_t len,
                 const char *printed, int line)
{
    lc_type t = ty(type);
    lc_str out = LC_STR_INIT;
    const lc_value *v = NULL;
    size_t pos = 0;
    int rc = lc_wire_decode(ctx, pool, &t, wire, len, &pos, &v);

    if (rc != LC_OK)
        fprintf(stderr, "    line %d: %s\n", line, lc_last_error(ctx));
    CHECK_INT(rc, LC_OK);
    if (v) {
        lc_value_print(v, &out);
        CHECK_STR(lc_str_cstr(&out), printed);
    }
    lc_str_free(&out);
}
#define dec(type, wire, printed) dec_(type, wire, strlen(wire), printed, \
                                      __LINE__)

static void bad_(const char *type, const char *wire, size_t len,
                 const char *msg, int line)
{
    lc_type t = ty(type);
    const lc_value *v = (const lc_value *)&t;
    size_t pos = 0;
    int rc = lc_wire_decode(ctx, pool, &t, wire, len, &pos, &v);

    if (rc != LC_ERR_PARSE)
        fprintf(stderr, "    line %d: decoded\n", line);
    CHECK_INT(rc, LC_ERR_PARSE);
    CHECK(v == NULL);
    CHECK_INT(pos, 0);
    CHECK_STR(lc_last_error(ctx), msg);
}
#define bad(type, wire, msg) bad_(type, wire, strlen(wire), msg, __LINE__)

static void test_decode(void)
{
    dec("int", "  \n-5\n", "-5");
    dec("int", "\t42", "42");
    dec("double", "inf", "Infinity");
    dec("double", "-inf", "-Infinity");
    dec("double", "nan", "NaN");
    dec("double", "1e+300", "1e+300");
    dec("double", "7", "7.0");
    dec("double", "-0", "-0.0");
    dec("char", "0", "\"\\u0000\"");
    dec("char", "255", "\"\\xFF\"");
    dec_("string", "3:a\0b", 5, "\"a\\u0000b\"", __LINE__);
    dec("string", "4:\xff\xfe\n ", "\"\\xFF\\xFE\\n \"");
    dec("string", "1: ", "\" \"");
    dec("int[][]", "2\n2 1 2\n0", "[[1,2],[]]");
    dec("TreeNode", "5 1 n 2 n n", "[1,null,2]");
    dec("TreeNode", "4 1 2 n n", "[1,2]");
    dec("NaryNode", "4 1 n 2 n", "[1,null,2]");
    dec("ListNode", "2 -1 2147483647", "[-1,2147483647]");
    dec("void", "anything", "null");

    /* Values one after another, as a program's inputs are written. */
    {
        const char *w = "3 2 7 11\n9\n";
        lc_type ta = ty("int[]"), tb = ty("int");
        const lc_value *a = NULL, *b = NULL;
        size_t pos = 0;

        CHECK_INT(lc_wire_decode(ctx, pool, &ta, w, strlen(w), &pos, &a),
                  LC_OK);
        CHECK_INT(pos, 8);
        CHECK_INT(lc_wire_decode(ctx, pool, &tb, w, strlen(w), &pos, &b),
                  LC_OK);
        CHECK_INT(pos, 10);
        CHECK(a && a->len == 3);
        CHECK(b && b->u.i == 9);
    }
}

static void test_decode_errors(void)
{
    bad("int", "", "byte 1: expected an int, got the end of the result");
    bad("int", "   ", "byte 4: expected an int, got the end of the result");
    bad("int", "x", "byte 1: expected an int, got 'x'");
    bad("int", "-", "byte 1: expected an int, got '-'");
    bad("int", "1x", "byte 1: expected an int, got '1x'");
    bad("int", "+1", "byte 1: expected an int, got '+1'");
    bad("int", "2147483648", "byte 1: expected an int, got '2147483648'");
    bad("int", "123456789012345678901234567890", "byte 1: expected an int, "
        "got '12345678901234567890...'");
    bad("long", "9223372036854775808", "byte 1: expected a long, got "
        "'9223372036854775808'");
    bad("uint32", "-1", "byte 1: expected a uint32, got '-1'");
    bad("bool", "2", "byte 1: expected a bool (0 or 1), got '2'");
    bad("bool", "true", "byte 1: expected a bool (0 or 1), got 'true'");
    bad("char", "256", "byte 1: expected a char (0 to 255), got '256'");
    bad("double", "abc", "byte 1: expected a double, got 'abc'");
    bad("double", "1e999", "byte 1: expected a double, got '1e999'");
    bad("double", "Infinity", "byte 1: expected a double, got 'Infinity'");
    bad("double", "1,5", "byte 1: expected a double, got '1,5'");
    bad("string", "abc", "byte 1: expected a string ('length:bytes'), got "
        "'abc'");
    bad("string", ":abc", "byte 1: expected a string ('length:bytes'), got "
        "':abc'");
    bad("string", "3", "byte 1: expected a string ('length:bytes'), got "
        "'3'");
    bad("string", "5:abc", "byte 1: string length 5 is longer than the rest "
        "of the result");
    bad("string", "2:abc", "byte 5: expected a space after the string's 2 "
        "bytes");
    bad("string", "99999999999999999999999:x", "byte 1: string length is "
        "too large");
    bad("int[]", "", "byte 1: expected an array length, got the end of the "
        "result");
    bad("int[]", "-1 5", "byte 1: expected an array length, got '-1'");
    bad("int[]", "3 1 2", "byte 1: array length 3 is longer than the rest "
        "of the result");
    bad("int[]", "99999999999 1", "byte 1: array length 99999999999 is "
        "longer than the rest of the result");
    bad("int[]", "2 1 x", "byte 5: expected an int, got 'x'");
    bad("int[][]", "1 1 n", "byte 5: expected an int, got 'n'");
    bad("ListNode", "2 1 n", "byte 5: expected a node value, got 'n'");
    bad("TreeNode", "2 1 x", "byte 5: expected a node value or 'n', got "
        "'x'");
    bad("TreeNode", "2 1 nn", "byte 5: expected a node value or 'n', got "
        "'nn'");
    bad("TreeNode", "1 n", "byte 1: item 0 of the tree: a tree can't start "
        "with null (write [] for an empty tree)");
    bad("TreeNode", "4 1 n n 2", "byte 1: item 3 of the tree: this item has "
        "no parent: every node before it already has two children");
    bad("NaryNode", "2 1 2", "byte 1: item 1 of the tree: an N-ary tree's "
        "root must be followed by null");
    bad("TreeNode", "1 2147483648", "byte 3: expected a node value or 'n', "
        "got '2147483648'");
}

static void test_bad_arguments(void)
{
    lc_type t = ty("int");
    const lc_value *v;
    size_t pos = 5;

    CHECK_INT(lc_wire_decode(ctx, pool, &t, "1", 1, &pos, &v),
              LC_ERR_INVALID);
    pos = 0;
    CHECK_INT(lc_wire_decode(ctx, NULL, &t, "1", 1, &pos, &v),
              LC_ERR_INVALID);
    CHECK_INT(lc_wire_decode(ctx, pool, NULL, "1", 1, &pos, &v),
              LC_ERR_INVALID);
    t.kind = LC_T_VOID;
    t.dims = 1;
    CHECK_INT(lc_wire_decode(ctx, pool, &t, "1", 1, &pos, &v),
              LC_ERR_INVALID);
}

/* Random token soup must never crash the decoder, and whatever decodes
 * must encode and decode again to the same value. */
static void test_fuzz(void)
{
    static const char *const types[] = {
        "int", "long", "double", "bool", "char", "string", "int[]",
        "int[][]", "string[]", "ListNode", "TreeNode", "NaryNode",
        "TreeNode[]", "double[]", "char[][]"
    };
    static const char *const tokens[] = {
        "0", "1", "2", "3", "-1", "n", "1:a", "2:ab", "0:", "3: \n ", "x",
        "1e5", "nan", "inf", "-0", "2147483648", "255", " ", "\n", ":"
    };
    unsigned long seed = 99;
    lc_str in = LC_STR_INIT, w = LC_STR_INIT, p1 = LC_STR_INIT,
           p2 = LC_STR_INIT;
    int iter, k, failures = 0;

    for (iter = 0; iter < 30000; iter++) {
        lc_type t;
        const lc_value *v, *again;
        size_t pos = 0, ntok;

        lc_str_clear(&in);
        seed = seed * 6364136223846793005UL + 1442695040888963407UL;
        ntok = 1 + (size_t)((seed >> 33) % 12);
        for (k = 0; k < (int)ntok; k++) {
            seed = seed * 6364136223846793005UL + 1442695040888963407UL;
            lc_str_puts(&in, tokens[(seed >> 33) %
                                    (sizeof tokens / sizeof tokens[0])]);
            lc_str_putc(&in, ' ');
        }
        t = ty(types[iter % (sizeof types / sizeof types[0])]);
        if (lc_wire_decode(ctx, pool, &t, in.data, in.len, &pos, &v) !=
            LC_OK)
            continue;
        CHECK(pos <= in.len);
        lc_str_clear(&w);
        lc_str_clear(&p1);
        lc_str_clear(&p2);
        pos = 0;
        if (lc_wire_encode(ctx, &t, v, &w) != LC_OK ||
            lc_wire_decode(ctx, pool, &t, w.data, w.len, &pos, &again) !=
                LC_OK) {
            if (failures++ < 5)
                fprintf(stderr, "    fuzz: '%s': %s\n", in.data,
                        lc_last_error(ctx));
            continue;
        }
        lc_value_print(v, &p1);
        lc_value_print(again, &p2);
        if (strcmp(lc_str_cstr(&p1), lc_str_cstr(&p2)) != 0 &&
            failures++ < 5)
            fprintf(stderr, "    fuzz: '%s': %s vs %s\n", in.data, p1.data,
                    p2.data);
        if (iter % 1000 == 999) {
            lc_pool_free(pool);
            pool = lc_pool_new();
        }
    }
    CHECK_INT(failures, 0);
    lc_str_free(&in);
    lc_str_free(&w);
    lc_str_free(&p1);
    lc_str_free(&p2);
}

int main(void)
{
    ctx = lc_open(NULL);
    pool = lc_pool_new();
    if (!ctx || !pool)
        return 1;
    RUN(test_encode);
    RUN(test_encode_errors);
    RUN(test_decode);
    RUN(test_decode_errors);
    RUN(test_bad_arguments);
    RUN(test_fuzz);
    lc_pool_free(pool);
    lc_close(ctx);
    return test_report();
}
