#include <math.h>
#include <string.h>

#include "internal.h"
#include "test.h"

/* Builds JSON with the calls in `body` and checks the result. */
#define JSON_CASE(expect_rc, expect_text, body)                           \
    do {                                                                  \
        lc_str out_ = LC_STR_INIT;                                        \
        lc_json j_;                                                       \
        lc_json *j = &j_;                                                 \
        lc_json_init(j, &out_);                                           \
        body;                                                             \
        CHECK_INT(lc_json_finish(j), expect_rc);                          \
        if ((expect_text) != NULL)                                        \
            CHECK_STR(lc_str_cstr(&out_), (expect_text));                 \
        lc_str_free(&out_);                                               \
    } while (0)

static void test_values(void)
{
    JSON_CASE(LC_OK, "null", lc_json_null(j));
    JSON_CASE(LC_OK, "true", lc_json_bool(j, 1));
    JSON_CASE(LC_OK, "false", lc_json_bool(j, 0));
    JSON_CASE(LC_OK, "-9223372036854775807", lc_json_int(j, -9223372036854775807LL));
    JSON_CASE(LC_OK, "0.1", lc_json_double(j, 0.1));
    JSON_CASE(LC_OK, "1e+300", lc_json_double(j, 1e300));
    JSON_CASE(LC_OK, "null", lc_json_double(j, NAN));
    JSON_CASE(LC_OK, "null", lc_json_double(j, INFINITY));
    JSON_CASE(LC_OK, "null", lc_json_string(j, NULL));
    JSON_CASE(LC_OK, "\"hi\"", lc_json_string(j, "hi"));
}

static void test_nesting(void)
{
    JSON_CASE(LC_OK, "{}", { lc_json_begin_object(j); lc_json_end_object(j); });
    JSON_CASE(LC_OK, "[]", { lc_json_begin_array(j); lc_json_end_array(j); });
    JSON_CASE(LC_OK, "{\"a\":1,\"b\":[1,\"x\",{}],\"c\":{\"d\":null}}", {
        lc_json_begin_object(j);
        lc_json_key(j, "a"); lc_json_int(j, 1);
        lc_json_key(j, "b");
        lc_json_begin_array(j);
        lc_json_int(j, 1); lc_json_string(j, "x");
        lc_json_begin_object(j); lc_json_end_object(j);
        lc_json_end_array(j);
        lc_json_key(j, "c");
        lc_json_begin_object(j); lc_json_key(j, "d"); lc_json_null(j);
        lc_json_end_object(j);
        lc_json_end_object(j);
    });
}

static void test_escaping(void)
{
    JSON_CASE(LC_OK, "\"q\\\" b\\\\ n\\n r\\r t\\t \\u0001 \\u001f\"",
              lc_json_string(j, "q\" b\\ n\n r\r t\t \x01 \x1f"));
    JSON_CASE(LC_OK, "\"a\\u0000b\"", lc_json_string_n(j, "a\0b", 3));
    /* Valid UTF-8 passes through; invalid bytes become U+FFFD. */
    JSON_CASE(LC_OK, "\"caf\xc3\xa9 \xe2\x89\xa4 \xf0\x9f\x98\x80\"",
              lc_json_string(j, "caf\xc3\xa9 \xe2\x89\xa4 \xf0\x9f\x98\x80"));
    JSON_CASE(LC_OK, "\"\\ufffdx\\ufffd\"", lc_json_string(j, "\xffx\xc3"));
    JSON_CASE(LC_OK, "\"\\ufffd\\ufffd\\ufffd\"", lc_json_string(j, "\xed\xa0\x80"));
    JSON_CASE(LC_OK, "\"\\ufffd\\ufffd\"", lc_json_string(j, "\xc0\xaf"));
    JSON_CASE(LC_OK, "{\"k\\\"\":1}", {
        lc_json_begin_object(j); lc_json_key(j, "k\""); lc_json_int(j, 1);
        lc_json_end_object(j);
    });
}

static void test_misuse(void)
{
    /* Nothing written. */
    JSON_CASE(LC_ERR_INVALID, NULL, (void)j);
    /* Two top-level values. */
    JSON_CASE(LC_ERR_INVALID, NULL, { lc_json_int(j, 1); lc_json_int(j, 2); });
    /* A value without a key in an object. */
    JSON_CASE(LC_ERR_INVALID, NULL, { lc_json_begin_object(j); lc_json_int(j, 1); });
    /* A key outside an object. */
    JSON_CASE(LC_ERR_INVALID, NULL, { lc_json_begin_array(j); lc_json_key(j, "a"); });
    /* A key without a value. */
    JSON_CASE(LC_ERR_INVALID, NULL, {
        lc_json_begin_object(j); lc_json_key(j, "a"); lc_json_end_object(j);
    });
    /* Mismatched end. */
    JSON_CASE(LC_ERR_INVALID, NULL, { lc_json_begin_object(j); lc_json_end_array(j); });
    /* Unclosed. */
    JSON_CASE(LC_ERR_INVALID, NULL, lc_json_begin_array(j));
    /* Too deep. */
    JSON_CASE(LC_ERR_INVALID, NULL, {
        int i_;
        for (i_ = 0; i_ < LC_JSON_MAX_DEPTH + 1; i_++)
            lc_json_begin_array(j);
    });
    /* Exactly the maximum depth is fine. */
    JSON_CASE(LC_OK, NULL, {
        int i_;
        for (i_ = 0; i_ < LC_JSON_MAX_DEPTH; i_++)
            lc_json_begin_array(j);
        for (i_ = 0; i_ < LC_JSON_MAX_DEPTH; i_++)
            lc_json_end_array(j);
    });
}

int main(void)
{
    RUN(test_values);
    RUN(test_nesting);
    RUN(test_escaping);
    RUN(test_misuse);
    return test_report();
}
