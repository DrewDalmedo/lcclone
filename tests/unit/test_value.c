#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* Tests for value.c: parsing literals, checking types, printing. */

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static lc_ctx *ctx;
static lc_pool *pool;

static const lc_type *type_of(const char *name, lc_type *buf)
{
    if (!name)
        return NULL;
    CHECK_INT(lc_type_parse(name, strlen(name), buf), LC_OK);
    return buf;
}

/* Parses text as `type` (NULL: untyped) and prints it. NULL on error. */
static const char *roundtrip(const char *type, const char *text, size_t len,
                             lc_str *out)
{
    lc_type t;
    const lc_value *v = NULL;

    lc_str_clear(out);
    if (lc_value_parse(ctx, pool, type_of(type, &t), text, len, &v) != LC_OK)
        return NULL;
    CHECK(v != NULL);
    CHECK_INT(lc_value_print(v, out), LC_OK);
    return lc_str_cstr(out);
}

/* `text` must parse as `type` and print as `printed` (NULL: as text). */
static void ok_(const char *type, const char *text, const char *printed,
                int line)
{
    lc_str out = LC_STR_INIT;
    const char *got = roundtrip(type, text, strlen(text), &out);

    if (!got)
        fprintf(stderr, "    line %d: %s '%s': %s\n", line, type ? type : "-",
                text, lc_last_error(ctx));
    CHECK_STR(got, printed ? printed : text);
    lc_str_free(&out);
}
#define ok(type, text, printed) ok_(type, text, printed, __LINE__)

static void bad_(const char *type, const char *text, const char *msg,
                 int line)
{
    lc_type t;
    const lc_value *v = (const lc_value *)&t;
    int rc = lc_value_parse(ctx, pool, type_of(type, &t), text, strlen(text),
                            &v);

    if (rc != LC_ERR_PARSE)
        fprintf(stderr, "    line %d: %s '%s' parsed\n", line,
                type ? type : "-", text);
    CHECK_INT(rc, LC_ERR_PARSE);
    CHECK(v == NULL);
    CHECK_STR(lc_last_error(ctx), msg);
}
#define bad(type, text, msg) bad_(type, text, msg, __LINE__)

static void test_untyped(void)
{
    ok(NULL, "0", NULL);
    ok(NULL, "-0", "0");
    ok(NULL, "42", NULL);
    ok(NULL, "-9223372036854775808", NULL);
    ok(NULL, "9223372036854775807", NULL);
    ok(NULL, "2.5", NULL);
    ok(NULL, "1E2", "100.0");
    ok(NULL, "true", NULL);
    ok(NULL, "false", NULL);
    ok(NULL, "null", NULL);
    ok(NULL, "\"\"", NULL);
    ok(NULL, "[]", NULL);
    ok(NULL, "[1,-2.5,true,false,null,\"x\",[[]]]", NULL);
    ok(NULL, " \t[ 1 ,\t2 ] \t", "[1,2]");
    ok(NULL, "[\n1,\r\n2]", "[1,2]");

    bad(NULL, "", "column 1: expected a value");
    bad(NULL, "   ", "column 4: expected a value");
    bad(NULL, "9223372036854775808", "column 1: integer 9223372036854775808 "
        "is out of range");
    bad(NULL, "01", "column 1: invalid number '01'");
    bad(NULL, "-", "column 1: invalid number '-'");
    bad(NULL, "1.", "column 1: invalid number '1.'");
    bad(NULL, "1e", "column 1: invalid number '1e'");
    bad(NULL, "1e+", "column 1: invalid number '1e+'");
    bad(NULL, "-.5", "column 1: invalid number '-.5'");
    bad(NULL, "+1", "column 1: unexpected character '+'");
    bad(NULL, ".5", "column 1: unexpected character '.'");
    bad(NULL, "nul", "column 1: unexpected character 'n'");
    bad(NULL, "truex", "column 1: unexpected character 't'");
    bad(NULL, "True", "column 1: unexpected character 'T'");
    bad(NULL, "[true,nullx]", "column 7: unexpected character 'n'");
    bad(NULL, "\x01", "column 1: unexpected byte 0x01");
    bad(NULL, "\xc3\xa9", "column 1: unexpected byte 0xC3");
    bad(NULL, "1 2", "column 3: unexpected text after the value");
    bad(NULL, "[]]", "column 3: unexpected text after the value");
    bad(NULL, "[1,2", "column 1: '[' is never closed");
    bad(NULL, "[[1,2]", "column 1: '[' is never closed");
    bad(NULL, "[1 2]", "column 4: expected ',' or ']'");
    bad(NULL, "[1,]", "column 4: expected a value after ','");
    bad(NULL, "[1, ]", "column 5: expected a value after ','");
    bad(NULL, "[,1]", "column 2: unexpected character ','");
    bad(NULL, "1e400", "column 1: 1e400 is out of range for double");
    bad(NULL, "12345678901234567890123456789012345678901234567890",
        "column 1: integer 123456789012345678901234567890... is out of "
        "range");
}

static void test_nesting_limit(void)
{
    char buf[2 * LC_VALUE_MAX_DEPTH + 8];
    char want[64];
    size_t i;

    for (i = 0; i < LC_VALUE_MAX_DEPTH; i++) {
        buf[i] = '[';
        buf[LC_VALUE_MAX_DEPTH + i] = ']';
    }
    buf[2 * LC_VALUE_MAX_DEPTH] = '\0';
    ok(NULL, buf, NULL);

    memmove(buf + 1, buf, strlen(buf) + 1);
    buf[0] = '[';
    strcat(buf, "]");
    snprintf(want, sizeof want, "column %d: arrays nested more than %d deep",
             LC_VALUE_MAX_DEPTH + 1, LC_VALUE_MAX_DEPTH);
    bad(NULL, buf, want);
}

static void test_integers(void)
{
    ok("int", "0", NULL);
    ok("int", "  7  ", "7");
    ok("int", "-2147483648", NULL);
    ok("int", "2147483647", NULL);
    bad("int", "2147483648", "column 1: 2147483648 is out of range for int");
    bad("int", "-2147483649", "column 1: -2147483649 is out of range for "
        "int");
    bad("int", "99999999999999999999", "column 1: 99999999999999999999 is "
        "out of range for int");
    bad("int", "1.5", "column 1: expected int, got 1.5");
    bad("int", "1e3", "column 1: expected int, got 1e3");
    bad("int", "\"1\"", "column 1: expected int, got a string");
    bad("int", "[1]", "column 1: expected int, got an array");
    bad("int", "true", "column 1: expected int, got true");
    bad("int", "null", "column 1: expected int, got null");
    bad("int", "x", "column 1: unexpected character 'x'");
    bad("int", "", "column 1: expected a value");
    bad("int", "007", "column 1: invalid number '007'");

    ok("long", "-9223372036854775808", NULL);
    ok("long", "9223372036854775807", NULL);
    bad("long", "9223372036854775808", "column 1: 9223372036854775808 is out "
        "of range for long");
    bad("long", "-9223372036854775809", "column 1: -9223372036854775809 is "
        "out of range for long");

    ok("uint32", "0", NULL);
    ok("uint32", "4294967295", NULL);
    bad("uint32", "4294967296", "column 1: 4294967296 is out of range for "
        "uint32");
    bad("uint32", "-1", "column 1: -1 is out of range for uint32");
}

static void test_doubles(void)
{
    ok("double", "2", "2.0");
    ok("double", "-3", "-3.0");
    ok("double", "2.5", NULL);
    ok("double", "-0", "-0.0");
    ok("double", "-0.0", NULL);
    ok("double", "0.1", NULL);
    ok("double", "0.30000000000000004", NULL);
    ok("double", "100000", "100000.0");
    ok("double", "1e20", "1e+20");
    ok("double", "1E-7", "1e-07");
    ok("double", "123456789012345678", "1.2345678901234568e+17");
    ok("double", "9007199254740993", "9007199254740992.0");
    ok("double", "1e-400", "0.0");
    ok("double", "4.9406564584124654e-324", "4.94065645841247e-324");
    ok("double", "1.7976931348623157e308", "1.7976931348623157e+308");
    bad("double", "1e309", "column 1: 1e309 is out of range for double");
    bad("double", "-1e309", "column 1: -1e309 is out of range for double");
    bad("double", "\"2.5\"", "column 1: expected double, got a string");
    bad("double", "NaN", "column 1: unexpected character 'N'");
    bad("double", "Infinity", "column 1: unexpected character 'I'");
    ok("double[]", "[1,2.5,-3]", "[1.0,2.5,-3.0]");
    ok("double[][]", "[[0.5],[]]", NULL);
}

static void test_bool_char_string(void)
{
    ok("bool", "true", NULL);
    ok("bool", "false", NULL);
    bad("bool", "1", "column 1: expected bool, got 1");
    bad("bool", "\"true\"", "column 1: expected bool, got a string");
    bad("bool", "null", "column 1: expected bool, got null");

    ok("char", "\"a\"", NULL);
    ok("char", "\" \"", NULL);
    ok("char", "\"\\\"\"", NULL);
    ok("char", "\"\\n\"", NULL);
    ok("char", "\"\\u0041\"", "\"A\"");
    bad("char", "\"ab\"", "column 1: expected char, a string of one ASCII "
        "character such as \"a\"");
    bad("char", "\"\"", "column 1: expected char, a string of one ASCII "
        "character such as \"a\"");
    bad("char", "\"\xc3\xa9\"", "column 1: expected char, a string of one "
        "ASCII character such as \"a\"");
    bad("char", "97", "column 1: expected char, got 97");
    bad("char", "'a'", "column 1: unexpected character '''");
    ok("char[]", "[\"h\",\"i\"]", NULL);
    ok("char[][]", "[[\"5\",\".\"],[\"a\",\"b\"]]", NULL);

    ok("string", "\"a b\"", NULL);
    ok("string", "\"\"", NULL);
    ok("string", "\"caf\xc3\xa9 \xe2\x82\xac \xf0\x9f\x98\x80\"", NULL);
    ok("string", "\"\\u00e9\\u20AC\"", "\"\xc3\xa9\xe2\x82\xac\"");
    ok("string", "\"\\ud83d\\ude00\"", "\"\xf0\x9f\x98\x80\"");
    ok("string", "\"\\/\"", "\"/\"");
    ok("string", "\"\\\\\\\"\"", NULL);
    ok("string", "\"\\b\\f\\n\\r\\t\"", NULL);
    ok("string", "\"\\u0001\\u001f\"", "\"\\u0001\\u001F\"");
    ok("string", "\"\\u007f\"", "\"\\u007F\"");
    ok("string", "\"\x7f\"", "\"\\u007F\"");
    ok("string[]", "[\"ab\",\"c\"]", NULL);
    ok("string[][]", "[[\"a\"],[]]", NULL);

    bad("string", "\"abc", "column 1: string has no closing quote");
    bad("string", "\"abc\\\"", "column 1: string has no closing quote");
    bad("string", "\"a\\", "column 1: string has no closing quote");
    bad("string", "\"\\x41\"", "column 2: unknown escape '\\x'");
    bad("string", "\"\\'\"", "column 2: unknown escape '\\''");
    bad("string", "\"\\\x01\"", "column 2: unknown escape");
    bad("string", "\"\\u12\"", "column 2: \\u must be followed by four hex "
        "digits");
    bad("string", "\"\\u12g4\"", "column 2: \\u must be followed by four hex "
        "digits");
    bad("string", "\"x\\ud800\"", "column 3: unpaired surrogate \\ud800");
    bad("string", "\"\\ud800\\u0041\"", "column 2: unpaired surrogate "
        "\\ud800");
    bad("string", "\"\\udc00\"", "column 2: unpaired surrogate \\udc00");
    bad("string", "\"\\u0000\"", "column 2: \\u0000 is not allowed (strings "
        "can't hold a NUL byte)");
    bad("string", "\"a\tb\"", "column 3: control character in a string "
        "(write it as an escape such as \\n or \\u0009)");
    bad("string", "\"\xff\"", "column 2: invalid UTF-8 in a string");
    bad("string", "\"\xc0\xaf\"", "column 2: invalid UTF-8 in a string");
    bad("string", "\"\xe0\x80\xaf\"", "column 2: invalid UTF-8 in a string");
    bad("string", "\"\xed\xa0\x80\"", "column 2: invalid UTF-8 in a string");
    bad("string", "\"\xf4\x90\x80\x80\"", "column 2: invalid UTF-8 in a "
        "string");
    bad("string", "\"ab\xc3\"", "column 4: invalid UTF-8 in a string");
    bad("string", "abc", "column 1: unexpected character 'a'");

    /* A string with a NUL byte in the text itself: the length counts. */
    {
        const lc_value *v;
        CHECK_INT(lc_value_parse(ctx, pool, NULL, "\"a\0b\"", 5, &v),
                  LC_ERR_PARSE);
        CHECK_STR(lc_last_error(ctx), "column 3: control character in a "
                  "string (write it as an escape such as \\n or \\u0000)");
    }
}

static void test_arrays(void)
{
    ok("int[]", "[]", NULL);
    ok("int[]", "[2,7,11]", NULL);
    ok("int[][]", "[[1,2],[3],[]]", NULL);
    bad("int[]", "1", "column 1: expected int[], got 1");
    bad("int[]", "null", "column 1: expected int[], got null");
    bad("int[][]", "[1]", "column 2: expected int[], got 1");
    bad("int[]", "[1,null]", "column 4: expected int, got null");
    bad("int[]", "[1,\"2\"]", "column 4: expected int, got a string");
    bad("int[]", "[1,2147483648]", "column 4: 2147483648 is out of range "
        "for int");
    bad("int[][]", "[[1],[2,]]", "column 9: expected a value after ','");
    bad("string[]", "[\"a\",[\"b\"]]", "column 6: expected string, got an "
        "array");
}

static void test_lists_and_trees(void)
{
    ok("ListNode", "[]", NULL);
    ok("ListNode", "[1,2,3]", NULL);
    ok("ListNode[]", "[[1,4,5],[],[2]]", NULL);
    bad("ListNode", "null", "column 1: expected ListNode, got null");
    bad("ListNode", "[1,null]", "column 4: expected int, got null");
    bad("ListNode", "[1,2.5]", "column 4: expected int, got 2.5");

    ok("TreeNode", "[]", NULL);
    ok("TreeNode", "[1]", NULL);
    ok("TreeNode", "[1,null,2]", NULL);
    ok("TreeNode", "[5,3,8,1,null,null,9,null,2]", NULL);
    ok("TreeNode", "[1,2,null,3,null,4,null,5]", NULL);
    ok("TreeNode", "[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]", NULL);
    /* Trailing nulls are dropped, as LeetCode prints trees. */
    ok("TreeNode", "[1,null,2,null,null]", "[1,null,2]");
    ok("TreeNode", "[1,2,3,null,null,null,null]", "[1,2,3]");
    ok("TreeNode", "[1,null]", "[1]");
    ok("TreeNode[]", "[[1,null,2],[]]", NULL);
    bad("TreeNode", "[null]", "column 2: a tree can't start with null "
        "(write [] for an empty tree)");
    bad("TreeNode", "[null,1]", "column 2: a tree can't start with null "
        "(write [] for an empty tree)");
    bad("TreeNode", "[1,null,null,2]", "column 14: this item has no parent: "
        "every node before it already has two children");
    bad("TreeNode", "[1,2,3,null,null,null,null,4]", "column 28: this item "
        "has no parent: every node before it already has two children");
    bad("TreeNode", "[1,null,null,null]", "column 14: this item has no "
        "parent: every node before it already has two children");
    bad("TreeNode", "[2147483648]", "column 2: 2147483648 is out of range "
        "for int");
    bad("TreeNode", "[1,\"x\"]", "column 4: expected int or null, got a "
        "string");
    bad("TreeNode", "1", "column 1: expected TreeNode, got 1");

    ok("NaryNode", "[]", NULL);
    ok("NaryNode", "[1]", NULL);
    ok("NaryNode", "[1,null]", "[1]");
    ok("NaryNode", "[1,null,3,2,4,null,5,6]", NULL);
    ok("NaryNode", "[1,null,2,3,4,5,null,null,6,7,null,8,null,9,10,null,null,"
       "11,null,12,null,13,null,null,14]", NULL);
    ok("NaryNode", "[1,null,2,null,3,null]", "[1,null,2,null,3]");
    ok("NaryNode", "[1,null,2,3,null,null,4]", NULL);
    bad("NaryNode", "[null]", "column 2: a tree can't start with null "
        "(write [] for an empty tree)");
    bad("NaryNode", "[1,2]", "column 4: an N-ary tree's root must be "
        "followed by null");
    bad("NaryNode", "[1,null,null]", "column 9: this null starts the "
        "children of a node that doesn't exist");
    bad("NaryNode", "[1,null,2,null,null,null]", "column 16: this null "
        "starts the children of a node that doesn't exist");
    bad("NaryNode", "[1,null,2,null,3,null,null]", "column 23: this null "
        "starts the children of a node that doesn't exist");
}

static void test_printer(void)
{
    lc_str out = LC_STR_INIT;
    lc_value v, items[3];

    memset(&v, 0, sizeof v);
    v.kind = LC_V_STRING;
    v.u.s = "a\xff" "b\xc3";
    v.len = 4;
    CHECK_INT(lc_value_print(&v, &out), LC_OK);
    CHECK_STR(lc_str_cstr(&out), "\"a\\xFFb\\xC3\"");

    lc_str_clear(&out);
    v.u.s = "x\0y";
    v.len = 3;
    CHECK_INT(lc_value_print(&v, &out), LC_OK);
    CHECK_STR(lc_str_cstr(&out), "\"x\\u0000y\"");

    memset(items, 0, sizeof items);
    items[0].kind = LC_V_FLOAT;
    items[0].u.f = NAN;
    items[1].kind = LC_V_FLOAT;
    items[1].u.f = INFINITY;
    items[2].kind = LC_V_FLOAT;
    items[2].u.f = -INFINITY;
    v.kind = LC_V_ARRAY;
    v.len = 3;
    v.u.items = items;
    lc_str_clear(&out);
    CHECK_INT(lc_value_print(&v, &out), LC_OK);
    CHECK_STR(lc_str_cstr(&out), "[NaN,Infinity,-Infinity]");

    CHECK_INT(lc_value_print(NULL, &out), LC_ERR_INVALID);
    lc_str_free(&out);
}

static void test_numbers(void)
{
    char buf[LC_DOUBLE_BUF];
    double d;
    int is_int;

    CHECK_INT(lc_scan_number("12,", 3, &is_int), 2);
    CHECK_INT(is_int, 1);
    CHECK_INT(lc_scan_number("-0.5e+3]", 8, &is_int), 7);
    CHECK_INT(is_int, 0);
    CHECK_INT(lc_scan_number("0x1", 3, &is_int), 1);
    CHECK_INT(lc_scan_number("00", 2, &is_int), 0);
    CHECK_INT(lc_scan_number("", 0, &is_int), 0);
    CHECK_INT(lc_scan_number("12", 1, &is_int), 1);

    CHECK_INT(lc_strtod("2.5", 3, &d), 0);
    CHECK(d == 2.5);
    CHECK_INT(lc_strtod("12", 1, &d), 0);
    CHECK(d == 1);
    CHECK_INT(lc_strtod("1e400", 5, &d), -1);
    {
        char longnum[500]; /* longer than lc_strtod takes */
        memset(longnum, '1', sizeof longnum);
        CHECK_INT(lc_strtod(longnum, sizeof longnum, &d), -1);
    }

    lc_fmt_double(0.1, buf);
    CHECK_STR(buf, "0.1");
    lc_fmt_double(1.0 / 3, buf);
    CHECK_STR(buf, "0.3333333333333333");
    lc_fmt_double(2.0 / 3, buf);
    CHECK_STR(buf, "0.6666666666666666");
    lc_fmt_double(1e300, buf);
    CHECK_STR(buf, "1e+300");
    lc_fmt_double(-0.0, buf);
    CHECK_STR(buf, "-0");
    lc_fmt_double(-1.7976931348623157e308, buf);
    CHECK_STR(buf, "-1.7976931348623157e+308");
    CHECK(strlen(buf) < LC_DOUBLE_BUF);
    lc_fmt_double(INFINITY, buf);
    CHECK_STR(buf, "inf");
    lc_fmt_double(-INFINITY, buf);
    CHECK_STR(buf, "-inf");
    lc_fmt_double(NAN, buf);
    CHECK_STR(buf, "nan");
}

/* A locale whose decimal point is a comma, or NULL. */
static const char *comma_locale(void)
{
    static const char *const names[] = {
        "de_DE.UTF-8", "de_DE.utf8", "fr_FR.UTF-8", "fr_FR.utf8", "de_DE",
        "fr_FR", "nl_NL.UTF-8", "ru_RU.UTF-8"
    };
    char probe[16];
    size_t i;

    for (i = 0; i < sizeof names / sizeof names[0]; i++) {
        if (!setlocale(LC_NUMERIC, names[i]))
            continue;
        snprintf(probe, sizeof probe, "%.1f", 0.5);
        if (strcmp(probe, "0,5") == 0)
            return names[i];
    }
    setlocale(LC_NUMERIC, "C");
    return NULL;
}

static void test_locale(void)
{
    char buf[LC_DOUBLE_BUF];
    double d;

    if (!comma_locale()) {
        printf("note: skipped the decimal-comma tests (no such locale "
               "installed)\n");
        return;
    }
    ok("double", "2.5", NULL);
    ok("double", "1.25e3", "1250.0");
    ok("double[]", "[0.1,2]", "[0.1,2.0]");
    CHECK_INT(lc_strtod("0.75", 4, &d), 0);
    CHECK(d == 0.75);
    lc_fmt_double(0.1, buf);
    CHECK_STR(buf, "0.1");
    lc_fmt_double(-1.5e-10, buf);
    CHECK_STR(buf, "-1.5e-10");
    /* A comma is not a decimal point in a literal, whatever the locale. */
    bad("double", "2,5", "column 2: unexpected text after the value");
    setlocale(LC_NUMERIC, "C");
}

static void test_big_input(void)
{
    lc_str text = LC_STR_INIT;
    const lc_value *v;
    lc_type t = {LC_T_INT, 1};
    int i;

    lc_str_putc(&text, '[');
    for (i = 0; i < 200000; i++)
        lc_str_printf(&text, "%s%d", i ? "," : "", i - 100000);
    lc_str_putc(&text, ']');
    CHECK_INT(lc_value_parse(ctx, pool, &t, text.data, text.len, &v), LC_OK);
    if (v) {
        CHECK_INT(v->len, 200000);
        CHECK_INT(v->u.items[0].u.i, -100000);
        CHECK_INT(v->u.items[199999].u.i, 99999);
    }
    lc_str_free(&text);
}

static void test_bad_arguments(void)
{
    const lc_value *v = NULL;
    lc_type t = {LC_T_VOID, 0};

    CHECK_INT(lc_value_parse(ctx, pool, &t, "1", 1, &v), LC_ERR_INVALID);
    t.kind = LC_T_INT;
    t.dims = 3;
    CHECK_INT(lc_value_parse(ctx, pool, &t, "1", 1, &v), LC_ERR_INVALID);
    CHECK_INT(lc_value_parse(ctx, NULL, NULL, "1", 1, &v), LC_ERR_INVALID);
    CHECK_INT(lc_value_parse(ctx, pool, NULL, NULL, 1, &v), LC_ERR_INVALID);
    CHECK_INT(lc_value_parse(ctx, pool, NULL, NULL, 0, &v), LC_ERR_PARSE);
}

/* Parsing mutated literals must never crash, and whatever parses must
 * print as text that parses back to the same printed text. */
static void test_fuzz(void)
{
    static const char *const seeds[] = {
        "[1,2,3]", "[[1,2],[3]]", "[5,3,8,1,null,null,9,null,2]",
        "[1,null,3,2,4,null,5,6]", "[\"ab\",\"c\\n\\u00e9\"]", "-12.5e-3",
        "[[0.5,1e10],[]]", "\"\\ud83d\\ude00\"", "[true,false,null]"
    };
    static const char *const types[] = {
        NULL, "int[]", "int[][]", "TreeNode", "NaryNode", "string[]",
        "double[][]", "string", "ListNode", "long", "char[]", "bool[]"
    };
    static const char alphabet[] = "[]\",\\-.0123456789eEnultrfu \t\xc3\xa9";
    unsigned long seed = 12345;
    lc_str out = LC_STR_INIT, again = LC_STR_INIT;
    char buf[64];
    int iter, k, failures = 0;

    for (iter = 0; iter < 20000; iter++) {
        const char *s = seeds[iter % (sizeof seeds / sizeof seeds[0])];
        size_t len = strlen(s);
        int edits;

        memcpy(buf, s, len);
        seed = seed * 6364136223846793005UL + 1442695040888963407UL;
        edits = 1 + (int)((seed >> 33) % 3);
        for (k = 0; k < edits && len > 0; k++) {
            size_t pos;
            seed = seed * 6364136223846793005UL + 1442695040888963407UL;
            pos = (size_t)((seed >> 33) % len);
            if ((seed >> 20) % 7 == 0)
                buf[pos] = (char)((seed >> 40) & 0xFF);
            else
                buf[pos] = alphabet[(seed >> 40) % (sizeof alphabet - 1)];
        }
        for (k = 0; k < (int)(sizeof types / sizeof types[0]); k++) {
            const char *first = roundtrip(types[k], buf, len, &out);
            const char *second;
            if (!first)
                continue;
            second = roundtrip(types[k], first, out.len, &again);
            if (!second || strcmp(first, second) != 0) {
                if (failures++ < 5)
                    fprintf(stderr, "    fuzz: %s '%.*s' -> '%s' -> '%s'\n",
                            types[k] ? types[k] : "-", (int)len, buf, first,
                            second ? second : lc_last_error(ctx));
            }
        }
        if (iter % 1000 == 999) {
            lc_pool_free(pool);
            pool = lc_pool_new();
        }
    }
    CHECK_INT(failures, 0);
    lc_str_free(&out);
    lc_str_free(&again);
}

int main(void)
{
    ctx = lc_open(NULL);
    pool = lc_pool_new();
    if (!ctx || !pool)
        return 1;
    RUN(test_untyped);
    RUN(test_nesting_limit);
    RUN(test_integers);
    RUN(test_doubles);
    RUN(test_bool_char_string);
    RUN(test_arrays);
    RUN(test_lists_and_trees);
    RUN(test_printer);
    RUN(test_numbers);
    RUN(test_locale);
    RUN(test_big_input);
    RUN(test_bad_arguments);
    RUN(test_fuzz);
    lc_pool_free(pool);
    lc_close(ctx);
    return test_report();
}
