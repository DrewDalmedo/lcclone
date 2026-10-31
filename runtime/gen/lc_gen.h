/*
 * lc_gen.h: what a pack's gen.c can use without an #include
 * (docs/authoring.md, "gen.c"). lc builds gen.c as C11 with this file
 * first and lc_gen_main.c after it, into a program that prints test
 * cases.
 *
 * gen.c defines one function:
 *
 *     void gen(int small)
 *
 * which prints one case: one line per input, in the order of the
 * problem's parameters (then its hidden inputs), each a literal as in
 * tests.txt. `small` asks for a small case, quick for a slow brute-force
 * solution to work out; otherwise make a full-size one, up to the
 * problem's constraints.
 *
 * The random numbers come from rnd() and friends, seeded from the case's
 * seed, so a seed always gives the same case on every system. Don't use
 * rand() or the time: then cases would differ from machine to machine.
 *
 *     void gen(int small)
 *     {
 *         int n = small ? (int)rnd(2, 8) : 10000, i;
 *         int *nums = malloc(n * sizeof *nums);
 *
 *         for (i = 0; i < n; i++)
 *             nums[i] = (int)rnd(-1000000000, 1000000000);
 *         put_ints(nums, n);
 *         put_int(rnd(-2000000000, 2000000000));
 *         free(nums);
 *     }
 */
#ifndef LC_GEN_H
#define LC_GEN_H

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gen(int small);

/* ------------------------------------------------------------------ */
/* Random numbers                                                      */
/* ------------------------------------------------------------------ */

static uint64_t lc_gen_state;
static unsigned long long lc_gen_seed_value;

/* splitmix64: small, fast, and the same everywhere. */
static uint64_t rnd_u64(void)
{
    uint64_t z = (lc_gen_state += 0x9e3779b97f4a7c15ull);

    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
    return z ^ (z >> 31);
}

/* Starts the numbers for the case with this seed. */
static void lc_gen_start(unsigned long long seed)
{
    lc_gen_seed_value = seed;
    lc_gen_state = seed;
    lc_gen_state = rnd_u64(); /* so that seeds 1, 2, 3 start far apart */
}

/* The seed of the case being made. */
static unsigned long long gen_seed(void)
{
    return lc_gen_seed_value;
}

static void lc_gen_fail(const char *what)
{
    fprintf(stderr, "gen: %s\n", what);
    exit(3);
}

/* A whole number from lo to hi, both included, every one as likely. */
static long long rnd(long long lo, long long hi)
{
    uint64_t span, r, floor;

    if (hi < lo)
        lc_gen_fail("rnd(lo, hi) with lo > hi");
    span = (uint64_t)hi - (uint64_t)lo + 1;
    if (span == 0)
        return (long long)rnd_u64(); /* the whole range */
    /* Drop the few values that would make some results likelier. */
    floor = (0 - span) % span;
    do
        r = rnd_u64();
    while (r < floor);
    return (long long)((uint64_t)lo + r % span);
}

/* A number from lo up to (not including) hi. */
static double rnd_double(double lo, double hi)
{
    return lo + (hi - lo) * ((double)(rnd_u64() >> 11) / 9007199254740992.0);
}

/* 1 with a chance of `percent` in 100, else 0. */
static int rnd_chance(int percent)
{
    return rnd(1, 100) <= percent;
}

/* One of the characters of `chars` (not empty). */
static char rnd_char(const char *chars)
{
    size_t n = strlen(chars);

    if (n == 0)
        lc_gen_fail("rnd_char(\"\")");
    return chars[rnd(0, (long long)n - 1)];
}

/* Puts a[0..n) in a random order. */
static void rnd_shuffle(int *a, int n)
{
    int i;

    for (i = n - 1; i > 0; i--) {
        int k = (int)rnd(0, i), t = a[i];

        a[i] = a[k];
        a[k] = t;
    }
}

/* ------------------------------------------------------------------ */
/* Printing inputs: each put_ function prints one input line           */
/* ------------------------------------------------------------------ */

static void lc_gen_double(double v)
{
    char buf[40];
    int prec;

    if (v != v || v - v != 0)
        lc_gen_fail("put_double: a NaN or an infinity has no literal");
    /* The shortest text that reads back as v. */
    for (prec = 15; prec <= 17; prec++) {
        snprintf(buf, sizeof buf, "%.*g", prec, v);
        if (strtod(buf, NULL) == v)
            break;
    }
    fputs(buf, stdout);
}

static void lc_gen_string(const char *s, size_t n)
{
    size_t i;

    putchar('"');
    for (i = 0; i < n; i++) {
        unsigned char c = (unsigned char)s[i];

        if (c == '"' || c == '\\')
            printf("\\%c", c);
        else if (c == '\n')
            fputs("\\n", stdout);
        else if (c == '\t')
            fputs("\\t", stdout);
        else if (c < 0x20 || c == 0x7f)
            printf("\\u%04x", c);
        else
            putchar(c);
    }
    putchar('"');
}

static void put_int(long long v)
{
    printf("%lld\n", v);
}

static void put_double(double v)
{
    lc_gen_double(v);
    putchar('\n');
}

static void put_bool(int v)
{
    puts(v ? "true" : "false");
}

static void put_char(char c)
{
    lc_gen_string(&c, 1);
    putchar('\n');
}

static void put_string(const char *s)
{
    lc_gen_string(s, strlen(s));
    putchar('\n');
}

/* Any literal, as written, e.g. "[1,null,2]". */
static void put_raw(const char *literal)
{
    puts(literal);
}

static void put_ints(const int *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++)
        printf("%s%d", i ? "," : "", a[i]);
    puts("]");
}

static void put_longs(const long long *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++)
        printf("%s%lld", i ? "," : "", a[i]);
    puts("]");
}

static void put_doubles(const double *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++) {
        if (i)
            putchar(',');
        lc_gen_double(a[i]);
    }
    puts("]");
}

static void put_bools(const bool *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++)
        printf("%s%s", i ? "," : "", a[i] ? "true" : "false");
    puts("]");
}

/* A char[]: each character a one-character string. */
static void put_chars(const char *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++) {
        if (i)
            putchar(',');
        lc_gen_string(&a[i], 1);
    }
    puts("]");
}

static void put_strings(char *const *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++) {
        if (i)
            putchar(',');
        lc_gen_string(a[i], strlen(a[i]));
    }
    puts("]");
}

/* An int[][] of `rows` rows of `cols` ints, from a[rows * cols] by rows. */
static void put_int_grid(const int *a, int rows, int cols)
{
    int r, c;

    putchar('[');
    for (r = 0; r < rows; r++) {
        printf("%s[", r ? "," : "");
        for (c = 0; c < cols; c++)
            printf("%s%d", c ? "," : "", a[r * cols + c]);
        putchar(']');
    }
    puts("]");
}

/* A char[][], the same way. */
static void put_char_grid(const char *a, int rows, int cols)
{
    int r, c;

    putchar('[');
    for (r = 0; r < rows; r++) {
        printf("%s[", r ? "," : "");
        for (c = 0; c < cols; c++) {
            if (c)
                putchar(',');
            lc_gen_string(&a[r * cols + c], 1);
        }
        putchar(']');
    }
    puts("]");
}

#endif /* LC_GEN_H */
