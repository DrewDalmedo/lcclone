#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static void test_alignment_and_reuse(void)
{
    lc_arena a = {0};
    size_t i;

    for (i = 1; i < 200; i++) {
        char *p = lc_arena_alloc(&a, i);
        CHECK(p != NULL);
        CHECK(((uintptr_t)p % _Alignof(max_align_t)) == 0);
        memset(p, 0xAB, i);
    }
    lc_arena_free(&a);
    CHECK(a.head == NULL);
    /* The arena is reusable after free. */
    CHECK(lc_arena_alloc(&a, 10) != NULL);
    lc_arena_free(&a);
}

static void test_big_blocks(void)
{
    lc_arena a = {0};
    char *small1 = lc_arena_alloc(&a, 16);
    char *big = lc_arena_alloc(&a, 1 << 20);
    char *small2 = lc_arena_alloc(&a, 16);

    CHECK(small1 && big && small2);
    memset(big, 1, 1 << 20);
    /* A big allocation doesn't waste the current block. */
    CHECK(small2 == small1 + 16 || small2 == small1 + _Alignof(max_align_t) ||
          (size_t)(small2 - small1) <= 64);
    lc_arena_free(&a);
}

static void test_zero_and_overflow(void)
{
    lc_arena a = {0};
    int *z = lc_arena_zalloc(&a, 100 * sizeof(int));
    int i, allzero = 1;

    CHECK(z != NULL);
    for (i = 0; i < 100; i++)
        if (z[i])
            allzero = 0;
    CHECK(allzero);
    CHECK(lc_arena_alloc(&a, 0) != NULL);
    CHECK(lc_arena_alloc(&a, SIZE_MAX) == NULL);
    CHECK(lc_arena_array(&a, SIZE_MAX / 2, 4) == NULL);
    CHECK(lc_arena_array(&a, 3, 4) != NULL);
    lc_arena_free(&a);
}

static void test_strings(void)
{
    lc_arena a = {0};

    CHECK_STR(lc_arena_strdup(&a, "hello"), "hello");
    CHECK_STR(lc_arena_strndup(&a, "hello", 3), "hel");
    CHECK_STR(lc_arena_strndup(&a, "", 0), "");
    CHECK_STR(lc_arena_printf(&a, "%d-%s", 42, "x"), "42-x");
    lc_arena_free(&a);
}

static void test_grow(void)
{
    int *arr = NULL, *tmp;
    size_t cap = 0, i;

    for (i = 0; i < 1000; i++) {
        tmp = lc_grow(arr, &cap, i + 1, sizeof *arr);
        CHECK(tmp != NULL);
        if (!tmp)
            break;
        arr = tmp;
        arr[i] = (int)i;
    }
    CHECK(cap >= 1000);
    CHECK(arr[999] == 999);
    tmp = lc_grow(arr, &cap, SIZE_MAX, sizeof *arr);
    CHECK(tmp == NULL);
    CHECK(cap >= 1000); /* unchanged on failure */
    free(arr);
}

int main(void)
{
    RUN(test_alignment_and_reuse);
    RUN(test_big_blocks);
    RUN(test_zero_and_overflow);
    RUN(test_strings);
    RUN(test_grow);
    return test_report();
}
