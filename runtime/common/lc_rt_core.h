/*
 * lc_rt_core.h: the part of the harness runtime that C and C++ builds
 * share. It reads a case's inputs from stdin and writes the result to
 * fd 3, both in the wire format (docs/wire.md).
 *
 * lc places it after the solution in the same file, followed by
 * lc_rt.c or lc_rt.cpp and then the generated harness. So it is written in
 * the common subset of C11 and C++11, and every name in it is static and
 * starts with lc_ or LC_, which a solution has no reason to use.
 *
 * The input comes from lc, so a malformed one is a bug in lc; the harness
 * then stops with LC_RT_FAIL. It also stops that way when the solution
 * hands back something that can't be written, such as a NULL array.
 *
 * It does its I/O with stdio only, and includes no POSIX header such as
 * <unistd.h>: as on LeetCode, a solution may name its own functions read,
 * write, sleep or nice. Nor does it call any of those names, which would
 * then be the solution's functions. Its one POSIX function, fdopen, is in
 * <stdio.h>, which the preludes include anyway.
 */
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__GNUC__) || defined(__clang__)
#define LC_RT static __attribute__((unused))
#define LC_RT_NORETURN __attribute__((noreturn))
#define LC_RT_PRINTF(f, a) __attribute__((format(printf, f, a)))
#else
#define LC_RT static
#define LC_RT_NORETURN
#define LC_RT_PRINTF(f, a)
#endif

/* Exit status when the harness can't go on (EX_SOFTWARE). */
#define LC_RT_FAIL 70
/* The descriptor the result goes to. */
#define LC_RT_RESULT_FD 3

static char *lc_rt_in;         /* all of stdin, NUL-terminated */
static size_t lc_rt_in_len;
static size_t lc_rt_in_pos;
static char *lc_rt_out;        /* the result being built */
static size_t lc_rt_out_len;
static size_t lc_rt_out_cap;

LC_RT LC_RT_NORETURN LC_RT_PRINTF(1, 2) void lc_rt_fail(const char *fmt, ...)
{
    va_list ap;

    fflush(stdout);
    fputs("lc: ", stderr);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(LC_RT_FAIL);
}

/* calloc that never returns NULL; n may be 0. Even then it asks for no
 * elements, so that AddressSanitizer reports reading one. */
LC_RT void *lc_rt_alloc(size_t n, size_t size)
{
    void *p = calloc(n, size);

    if (!p && n == 0)
        p = calloc(1, size);
    if (!p)
        lc_rt_fail("out of memory");
    return p;
}

/* Reads all of stdin. Called first thing in main(). */
LC_RT void lc_rt_read_input(void)
{
    size_t cap = 65536;

    /* What the solution prints shows up even if it then crashes. */
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
    lc_rt_in = (char *)lc_rt_alloc(cap, 1);
    /* fread fills the buffer unless the input ends or can't be read. */
    while (!feof(stdin)) {
        if (cap - lc_rt_in_len < 2) {
            char *p = (char *)realloc(lc_rt_in, cap * 2);
            if (!p)
                lc_rt_fail("out of memory");
            lc_rt_in = p;
            cap *= 2;
        }
        lc_rt_in_len += fread(lc_rt_in + lc_rt_in_len, 1,
                              cap - lc_rt_in_len - 1, stdin);
        if (ferror(stdin)) {
            if (errno == EINTR) {
                clearerr(stdin);
                continue;
            }
            lc_rt_fail("cannot read the input: %s", strerror(errno));
        }
    }
    lc_rt_in[lc_rt_in_len] = '\0';
}

LC_RT int lc_rt_is_space(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

LC_RT void lc_rt_skip_space(void)
{
    while (lc_rt_in_pos < lc_rt_in_len &&
           lc_rt_is_space((unsigned char)lc_rt_in[lc_rt_in_pos]))
        lc_rt_in_pos++;
}

/* The next token, NUL-terminated in place (over the blank after it). */
LC_RT char *lc_rt_token(void)
{
    size_t start;

    lc_rt_skip_space();
    if (lc_rt_in_pos >= lc_rt_in_len)
        lc_rt_fail("the input ended early");
    start = lc_rt_in_pos;
    while (lc_rt_in_pos < lc_rt_in_len &&
           !lc_rt_is_space((unsigned char)lc_rt_in[lc_rt_in_pos]))
        lc_rt_in_pos++;
    lc_rt_in[lc_rt_in_pos] = '\0';
    if (lc_rt_in_pos < lc_rt_in_len)
        lc_rt_in_pos++;
    return lc_rt_in + start;
}

/* After the last input: nothing but blanks may be left. */
LC_RT void lc_rt_end_input(void)
{
    lc_rt_skip_space();
    if (lc_rt_in_pos < lc_rt_in_len)
        lc_rt_fail("unexpected input at byte %lu",
                   (unsigned long)lc_rt_in_pos + 1);
}

/* An integer token from lo to hi. */
LC_RT long long lc_rt_ll(long long lo, long long hi)
{
    char *tok = lc_rt_token(), *end;
    long long v;

    errno = 0;
    v = strtoll(tok, &end, 10);
    if (errno || end == tok || *end || v < lo || v > hi)
        lc_rt_fail("expected an integer from %lld to %lld, got '%.20s'", lo,
                   hi, tok);
    return v;
}

LC_RT double lc_rt_double(void)
{
    char *tok = lc_rt_token(), *end;
    double v = strtod(tok, &end);

    if (end == tok || *end)
        lc_rt_fail("expected a number, got '%.20s'", tok);
    return v;
}

LC_RT int lc_rt_bool(void)
{
    char *tok = lc_rt_token();

    if ((tok[0] != '0' && tok[0] != '1') || tok[1])
        lc_rt_fail("expected 0 or 1, got '%.20s'", tok);
    return tok[0] == '1';
}

/* A count of items or bytes, which can't be more than what is left of the
 * input, nor more than a C int can hold. */
LC_RT int lc_rt_count(void)
{
    long long n = lc_rt_ll(0, INT_MAX);

    if ((unsigned long long)n > lc_rt_in_len - lc_rt_in_pos)
        lc_rt_fail("a count of %lld is more than the input holds", n);
    return (int)n;
}

/* A string: its length, ':', then exactly that many bytes. The bytes are
 * copied into a new NUL-terminated buffer; *len gets the length. */
LC_RT char *lc_rt_string(size_t *len)
{
    size_t start, n = 0;
    char *s;

    lc_rt_skip_space();
    start = lc_rt_in_pos;
    while (lc_rt_in_pos < lc_rt_in_len && lc_rt_in[lc_rt_in_pos] >= '0' &&
           lc_rt_in[lc_rt_in_pos] <= '9' && n <= lc_rt_in_len) {
        n = n * 10 + (size_t)(lc_rt_in[lc_rt_in_pos] - '0');
        lc_rt_in_pos++;
    }
    if (lc_rt_in_pos == start || lc_rt_in_pos >= lc_rt_in_len ||
        lc_rt_in[lc_rt_in_pos] != ':' ||
        n > lc_rt_in_len - lc_rt_in_pos - 1)
        lc_rt_fail("malformed string at byte %lu", (unsigned long)start + 1);
    lc_rt_in_pos++;
    s = (char *)lc_rt_alloc(n + 1, 1);
    memcpy(s, lc_rt_in + lc_rt_in_pos, n);
    lc_rt_in_pos += n;
    if (lc_rt_in_pos < lc_rt_in_len &&
        !lc_rt_is_space((unsigned char)lc_rt_in[lc_rt_in_pos]))
        lc_rt_fail("malformed string at byte %lu", (unsigned long)start + 1);
    *len = n;
    return s;
}

/* ------------------------------------------------------------------ */
/* The result                                                          */
/* ------------------------------------------------------------------ */

LC_RT void lc_rt_put(const char *p, size_t n)
{
    if (lc_rt_out_cap - lc_rt_out_len < n + 2) {
        size_t cap = lc_rt_out_cap ? lc_rt_out_cap : 4096;
        char *q;

        while (cap - lc_rt_out_len < n + 2) {
            if (cap > (size_t)-1 / 2)
                lc_rt_fail("out of memory");
            cap *= 2;
        }
        q = (char *)realloc(lc_rt_out, cap);
        if (!q)
            lc_rt_fail("out of memory");
        lc_rt_out = q;
        lc_rt_out_cap = cap;
    }
    memcpy(lc_rt_out + lc_rt_out_len, p, n);
    lc_rt_out_len += n;
}

/* Starts a token: a space unless it is the first. */
LC_RT void lc_rt_sep(void)
{
    if (lc_rt_out_len)
        lc_rt_put(" ", 1);
}

LC_RT void lc_rt_put_ll(long long v)
{
    char buf[32];
    int n = snprintf(buf, sizeof buf, "%lld", v);

    lc_rt_sep();
    lc_rt_put(buf, (size_t)n);
}

LC_RT void lc_rt_put_ull(unsigned long long v)
{
    char buf[32];
    int n = snprintf(buf, sizeof buf, "%llu", v);

    lc_rt_sep();
    lc_rt_put(buf, (size_t)n);
}

/* %.17g reads back as the same double. A solution that changed the
 * locale could make the decimal point a comma, so that is put back. */
LC_RT void lc_rt_put_double(double v)
{
    char buf[64];
    int n, i;

    if (isnan(v))
        n = snprintf(buf, sizeof buf, "nan");
    else if (isinf(v))
        n = snprintf(buf, sizeof buf, "%s", v < 0 ? "-inf" : "inf");
    else
        n = snprintf(buf, sizeof buf, "%.17g", v);
    for (i = 0; i < n; i++)
        if (buf[i] == ',')
            buf[i] = '.';
    lc_rt_sep();
    lc_rt_put(buf, (size_t)n);
}

LC_RT void lc_rt_put_string(const char *s, size_t len)
{
    char buf[32];
    int n = snprintf(buf, sizeof buf, "%lu:", (unsigned long)len);

    lc_rt_sep();
    lc_rt_put(buf, (size_t)n);
    lc_rt_put(s, len);
}

/* `check: prefix`: the count k that `fn` returned must fit the array
 * `param`, which has `size` elements after the call. */
LC_RT void lc_rt_check_prefix(const char *fn, const char *param, long long k,
                              long long size)
{
    if (k < 0 || k > size)
        lc_rt_fail("%s returned %lld, but %s has %lld element%s", fn, k,
                   param, size, size == 1 ? "" : "s");
}

/* Writes the result on fd 3. Called last thing in main(). */
LC_RT void lc_rt_flush(void)
{
    FILE *f;

    lc_rt_put("\n", 1);
    f = fdopen(LC_RT_RESULT_FD, "w");
    if (!f || fwrite(lc_rt_out, 1, lc_rt_out_len, f) != lc_rt_out_len ||
        fclose(f) != 0)
        lc_rt_fail("cannot write the result on fd %d: %s", LC_RT_RESULT_FD,
                   strerror(errno));
}
