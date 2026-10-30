#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

void lc_str_init(lc_str *s)
{
    s->data = NULL;
    s->len = 0;
    s->cap = 0;
    s->failed = 0;
}

void lc_str_free(lc_str *s)
{
    free(s->data);
    lc_str_init(s);
}

void lc_str_clear(lc_str *s)
{
    s->len = 0;
    s->failed = 0;
    if (s->data)
        s->data[0] = '\0';
}

int lc_str_reserve(lc_str *s, size_t extra)
{
    size_t need, ncap;
    char *p;

    if (s->failed)
        return LC_ERR_NOMEM;
    if (extra > SIZE_MAX - 1 - s->len)
        goto fail;
    need = s->len + extra + 1;
    if (need <= s->cap)
        return LC_OK;
    ncap = s->cap ? s->cap : 64;
    while (ncap < need) {
        if (ncap > SIZE_MAX / 2) {
            ncap = need;
            break;
        }
        ncap *= 2;
    }
    p = realloc(s->data, ncap);
    if (!p)
        goto fail;
    if (!s->data)
        p[0] = '\0';
    s->data = p;
    s->cap = ncap;
    return LC_OK;
fail:
    s->failed = 1;
    return LC_ERR_NOMEM;
}

int lc_str_append(lc_str *s, const char *p, size_t n)
{
    if (lc_str_reserve(s, n) != LC_OK)
        return LC_ERR_NOMEM;
    if (n)
        memcpy(s->data + s->len, p, n);
    s->len += n;
    s->data[s->len] = '\0';
    return LC_OK;
}

int lc_str_puts(lc_str *s, const char *p)
{
    return lc_str_append(s, p, strlen(p));
}

int lc_str_putc(lc_str *s, int c)
{
    char ch = (char)c;
    return lc_str_append(s, &ch, 1);
}

int lc_str_vprintf(lc_str *s, const char *fmt, va_list ap)
{
    va_list ap2;
    int n;

    if (s->failed)
        return LC_ERR_NOMEM;
    va_copy(ap2, ap);
    n = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    if (n < 0) {
        s->failed = 1;
        return LC_ERR_NOMEM;
    }
    if (lc_str_reserve(s, (size_t)n) != LC_OK)
        return LC_ERR_NOMEM;
    vsnprintf(s->data + s->len, (size_t)n + 1, fmt, ap);
    s->len += (size_t)n;
    return LC_OK;
}

int lc_str_printf(lc_str *s, const char *fmt, ...)
{
    va_list ap;
    int rc;

    va_start(ap, fmt);
    rc = lc_str_vprintf(s, fmt, ap);
    va_end(ap);
    return rc;
}

const char *lc_str_cstr(const lc_str *s)
{
    return s->data ? s->data : "";
}
