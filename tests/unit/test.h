/*
 * test.h: a tiny assertion helper for the unit tests. No framework.
 *
 * Each suite is its own program:
 *
 *     #include "test.h"
 *     static void test_something(void) { CHECK(1 + 1 == 2); }
 *     int main(void) { RUN(test_something); return test_report(); }
 *
 * A failed check prints file:line and the expression, and the suite keeps
 * going. test_report() prints a summary and returns the exit status.
 *
 * Tests run from the repository root, so fixture paths such as
 * "tests/fixtures/problems" are relative to it. Scratch files go under
 * "tests/tmp/", which `make clean` removes.
 */
#ifndef LC_TEST_H
#define LC_TEST_H

#include <stdio.h>
#include <string.h>

static int test_checks;
static int test_failures;
static const char *test_current = "";

#define CHECK(cond)                                                       \
    do {                                                                  \
        test_checks++;                                                    \
        if (!(cond)) {                                                    \
            test_failures++;                                              \
            fprintf(stderr, "%s:%d: [%s] CHECK failed: %s\n", __FILE__,   \
                    __LINE__, test_current, #cond);                       \
        }                                                                 \
    } while (0)

#define CHECK_INT(a, b)                                                   \
    do {                                                                  \
        long long test_a_ = (long long)(a), test_b_ = (long long)(b);     \
        test_checks++;                                                    \
        if (test_a_ != test_b_) {                                         \
            test_failures++;                                              \
            fprintf(stderr, "%s:%d: [%s] CHECK_INT failed: %s == %s "     \
                    "(%lld vs %lld)\n", __FILE__, __LINE__, test_current, \
                    #a, #b, test_a_, test_b_);                            \
        }                                                                 \
    } while (0)

/* Compares two strings; either may be NULL. */
#define CHECK_STR(a, b)                                                   \
    do {                                                                  \
        const char *test_a_ = (a), *test_b_ = (b);                        \
        test_checks++;                                                    \
        if (!((test_a_ == NULL && test_b_ == NULL) ||                     \
              (test_a_ && test_b_ && strcmp(test_a_, test_b_) == 0))) {   \
            test_failures++;                                              \
            fprintf(stderr, "%s:%d: [%s] CHECK_STR failed: %s == %s\n"    \
                    "    got:      %s%s%s\n    expected: %s%s%s\n",       \
                    __FILE__, __LINE__, test_current, #a, #b,             \
                    test_a_ ? "\"" : "", test_a_ ? test_a_ : "NULL",      \
                    test_a_ ? "\"" : "", test_b_ ? "\"" : "",             \
                    test_b_ ? test_b_ : "NULL", test_b_ ? "\"" : "");     \
        }                                                                 \
    } while (0)

#define RUN(fn)                                                           \
    do {                                                                  \
        test_current = #fn;                                               \
        fn();                                                             \
    } while (0)

static int test_report_(const char *file)
{
    (void)test_current;
    if (test_failures) {
        fprintf(stderr, "%s: %d of %d checks FAILED\n", file, test_failures,
                test_checks);
        return 1;
    }
    printf("%s: %d checks passed\n", file, test_checks);
    return 0;
}

#define test_report() test_report_(__FILE__)

#endif /* LC_TEST_H */
