#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

static void fail(lc_json *j, int err)
{
    if (!j->err)
        j->err = err;
}

static void put(lc_json *j, const char *s, size_t n)
{
    if (!j->err && lc_str_append(j->out, s, n) != LC_OK)
        fail(j, LC_ERR_NOMEM);
}

static void putch(lc_json *j, char c)
{
    put(j, &c, 1);
}

/* Call before writing a value. Returns 1 if the value may be written. */
static int begin_value(lc_json *j)
{
    if (j->err)
        return 0;
    if (j->depth == 0) {
        if (j->done) {
            fail(j, LC_ERR_INVALID);
            return 0;
        }
        return 1;
    }
    if (j->kind[j->depth - 1] == 'o') {
        if (!j->after_key) {
            fail(j, LC_ERR_INVALID);
            return 0;
        }
        j->after_key = 0;
        return 1;
    }
    if (j->count[j->depth - 1])
        putch(j, ',');
    j->count[j->depth - 1] = 1;
    return !j->err;
}

static void end_value(lc_json *j)
{
    if (j->depth == 0)
        j->done = 1;
}

/* Length of a valid UTF-8 sequence at s (n bytes available), or 0. */
static size_t utf8_seq(const unsigned char *s, size_t n)
{
    unsigned char c = s[0];
    size_t len, i;
    unsigned char lo = 0x80, hi = 0xBF;

    if (c >= 0xC2 && c <= 0xDF)
        len = 2;
    else if (c >= 0xE0 && c <= 0xEF) {
        len = 3;
        if (c == 0xE0)
            lo = 0xA0;
        else if (c == 0xED)
            hi = 0x9F; /* no UTF-16 surrogates */
    } else if (c >= 0xF0 && c <= 0xF4) {
        len = 4;
        if (c == 0xF0)
            lo = 0x90;
        else if (c == 0xF4)
            hi = 0x8F;
    } else
        return 0;
    if (n < len)
        return 0;
    if (s[1] < lo || s[1] > hi)
        return 0;
    for (i = 2; i < len; i++)
        if (s[i] < 0x80 || s[i] > 0xBF)
            return 0;
    return len;
}

static void put_string(lc_json *j, const char *str, size_t n)
{
    const unsigned char *s = (const unsigned char *)str;
    size_t i = 0, run = 0;

    putch(j, '"');
    while (i < n && !j->err) {
        unsigned char c = s[i];
        const char *esc = NULL;
        char buf[8];
        size_t seq;

        if (c >= 0x20 && c < 0x80 && c != '"' && c != '\\') {
            i++;
            continue;
        }
        put(j, (const char *)s + run, i - run);
        if (c < 0x80) {
            switch (c) {
            case '"': esc = "\\\""; break;
            case '\\': esc = "\\\\"; break;
            case '\n': esc = "\\n"; break;
            case '\r': esc = "\\r"; break;
            case '\t': esc = "\\t"; break;
            case '\b': esc = "\\b"; break;
            case '\f': esc = "\\f"; break;
            default:
                snprintf(buf, sizeof buf, "\\u%04x", c);
                esc = buf;
            }
            put(j, esc, strlen(esc));
            i++;
        } else if ((seq = utf8_seq(s + i, n - i)) != 0) {
            put(j, (const char *)s + i, seq);
            i += seq;
        } else {
            put(j, "\\ufffd", 6);
            i++;
        }
        run = i;
    }
    put(j, (const char *)s + run, i - run);
    putch(j, '"');
}

void lc_json_init(lc_json *j, lc_str *out)
{
    memset(j, 0, sizeof *j);
    j->out = out;
}

static void begin_container(lc_json *j, char kind)
{
    if (!begin_value(j))
        return;
    if (j->depth >= LC_JSON_MAX_DEPTH) {
        fail(j, LC_ERR_INVALID);
        return;
    }
    putch(j, kind == 'o' ? '{' : '[');
    j->kind[j->depth] = (unsigned char)kind;
    j->count[j->depth] = 0;
    j->depth++;
}

static void end_container(lc_json *j, char kind)
{
    if (j->err)
        return;
    if (j->depth == 0 || j->kind[j->depth - 1] != (unsigned char)kind ||
        j->after_key) {
        fail(j, LC_ERR_INVALID);
        return;
    }
    putch(j, kind == 'o' ? '}' : ']');
    j->depth--;
    end_value(j);
}

void lc_json_begin_object(lc_json *j) { begin_container(j, 'o'); }
void lc_json_end_object(lc_json *j) { end_container(j, 'o'); }
void lc_json_begin_array(lc_json *j) { begin_container(j, 'a'); }
void lc_json_end_array(lc_json *j) { end_container(j, 'a'); }

void lc_json_key(lc_json *j, const char *key)
{
    if (j->err)
        return;
    if (j->depth == 0 || j->kind[j->depth - 1] != 'o' || j->after_key ||
        !key) {
        fail(j, LC_ERR_INVALID);
        return;
    }
    if (j->count[j->depth - 1])
        putch(j, ',');
    j->count[j->depth - 1] = 1;
    put_string(j, key, strlen(key));
    putch(j, ':');
    j->after_key = 1;
}

void lc_json_string_n(lc_json *j, const char *s, size_t n)
{
    if (!s) {
        lc_json_null(j);
        return;
    }
    if (!begin_value(j))
        return;
    put_string(j, s, n);
    end_value(j);
}

void lc_json_string(lc_json *j, const char *s)
{
    lc_json_string_n(j, s, s ? strlen(s) : 0);
}

void lc_json_int(lc_json *j, long long v)
{
    char buf[32];

    if (!begin_value(j))
        return;
    snprintf(buf, sizeof buf, "%lld", v);
    put(j, buf, strlen(buf));
    end_value(j);
}

void lc_json_double(lc_json *j, double v)
{
    char buf[40], *p;

    if (!isfinite(v)) {
        lc_json_null(j);
        return;
    }
    if (!begin_value(j))
        return;
    /* Shortest of %.15g / %.17g that reads back as the same double. */
    snprintf(buf, sizeof buf, "%.15g", v);
    if (strtod(buf, NULL) != v)
        snprintf(buf, sizeof buf, "%.17g", v);
    /* A front-end may have set a locale with a decimal comma. */
    for (p = buf; *p; p++)
        if (*p == ',')
            *p = '.';
    put(j, buf, strlen(buf));
    end_value(j);
}

void lc_json_bool(lc_json *j, int v)
{
    if (!begin_value(j))
        return;
    if (v)
        put(j, "true", 4);
    else
        put(j, "false", 5);
    end_value(j);
}

void lc_json_null(lc_json *j)
{
    if (!begin_value(j))
        return;
    put(j, "null", 4);
    end_value(j);
}

int lc_json_finish(lc_json *j)
{
    if (j->err)
        return j->err;
    if (j->out->failed)
        return LC_ERR_NOMEM;
    if (j->depth != 0 || !j->done || j->after_key)
        return LC_ERR_INVALID;
    return LC_OK;
}
