#include <stdint.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static void test_basic(void)
{
    lc_str s = LC_STR_INIT;

    CHECK_STR(lc_str_cstr(&s), "");
    CHECK_INT(lc_str_puts(&s, "abc"), LC_OK);
    CHECK_INT(lc_str_putc(&s, 'd'), LC_OK);
    CHECK_INT(lc_str_append(&s, "efgh", 2), LC_OK);
    CHECK_INT(lc_str_printf(&s, "-%d-%s", 7, "x"), LC_OK);
    CHECK_STR(s.data, "abcdef-7-x");
    CHECK_INT(s.len, 10);
    lc_str_clear(&s);
    CHECK_INT(s.len, 0);
    CHECK_STR(lc_str_cstr(&s), "");
    lc_str_free(&s);
    CHECK(s.data == NULL && s.len == 0 && s.cap == 0);
}

static void test_growth(void)
{
    lc_str s = LC_STR_INIT;
    int i;

    for (i = 0; i < 10000; i++)
        lc_str_printf(&s, "%d,", i % 10);
    CHECK(!s.failed);
    CHECK_INT(s.len, 20000);
    CHECK(s.data[s.len] == '\0');
    lc_str_free(&s);
}

static void test_embedded_nul(void)
{
    lc_str s = LC_STR_INIT;

    lc_str_append(&s, "a\0b", 3);
    CHECK_INT(s.len, 3);
    CHECK(memcmp(s.data, "a\0b", 4) == 0);
    lc_str_free(&s);
}

static void test_sticky_failure(void)
{
    lc_str s = LC_STR_INIT;

    lc_str_puts(&s, "x");
    CHECK_INT(lc_str_reserve(&s, SIZE_MAX), LC_ERR_NOMEM);
    CHECK(s.failed);
    CHECK_INT(lc_str_puts(&s, "more"), LC_ERR_NOMEM);
    CHECK_STR(s.data, "x");
    lc_str_clear(&s);
    CHECK(!s.failed);
    CHECK_INT(lc_str_puts(&s, "ok"), LC_OK);
    CHECK_STR(s.data, "ok");
    lc_str_free(&s);
}

int main(void)
{
    RUN(test_basic);
    RUN(test_growth);
    RUN(test_embedded_nul);
    RUN(test_sticky_failure);
    return test_report();
}
