#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

#define ARENA_BLOCK_SIZE ((size_t)16 * 1024)
#define ARENA_ALIGN (_Alignof(max_align_t))

struct lc_arena_block {
    lc_arena_block *next;
    size_t used;
    size_t cap;
    max_align_t data[]; /* keeps data aligned for any type */
};

static size_t round_up(size_t n)
{
    return (n + (ARENA_ALIGN - 1)) & ~(size_t)(ARENA_ALIGN - 1);
}

void *lc_arena_alloc(lc_arena *a, size_t n)
{
    lc_arena_block *b = a->head;
    size_t need;

    if (n == 0)
        n = 1;
    if (n > SIZE_MAX - ARENA_ALIGN)
        return NULL;
    need = round_up(n);

    if (b && b->cap - b->used >= need) {
        void *p = (char *)b->data + b->used;
        b->used += need;
        return p;
    }

    {
        size_t cap = need > ARENA_BLOCK_SIZE / 4 ? need : ARENA_BLOCK_SIZE;
        lc_arena_block *nb;

        if (cap > SIZE_MAX - sizeof(lc_arena_block))
            return NULL;
        nb = malloc(sizeof(lc_arena_block) + cap);
        if (!nb)
            return NULL;
        nb->cap = cap;
        nb->used = need;
        if (b && cap != ARENA_BLOCK_SIZE) {
            /* A big one-off block: keep the current block at the head so
             * its free space is still used. */
            nb->next = b->next;
            b->next = nb;
        } else {
            nb->next = b;
            a->head = nb;
        }
        return nb->data;
    }
}

void *lc_arena_zalloc(lc_arena *a, size_t n)
{
    void *p = lc_arena_alloc(a, n);
    if (p)
        memset(p, 0, n ? n : 1);
    return p;
}

void *lc_arena_array(lc_arena *a, size_t count, size_t size)
{
    if (size && count > SIZE_MAX / size)
        return NULL;
    return lc_arena_zalloc(a, count * size);
}

char *lc_arena_strndup(lc_arena *a, const char *s, size_t n)
{
    char *p;

    if (n == SIZE_MAX)
        return NULL;
    p = lc_arena_alloc(a, n + 1);
    if (!p)
        return NULL;
    if (n)
        memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

char *lc_arena_strdup(lc_arena *a, const char *s)
{
    return lc_arena_strndup(a, s, strlen(s));
}

char *lc_arena_vprintf(lc_arena *a, const char *fmt, va_list ap)
{
    va_list ap2;
    int n;
    char *p;

    va_copy(ap2, ap);
    n = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    if (n < 0)
        return NULL;
    p = lc_arena_alloc(a, (size_t)n + 1);
    if (!p)
        return NULL;
    vsnprintf(p, (size_t)n + 1, fmt, ap);
    return p;
}

char *lc_arena_printf(lc_arena *a, const char *fmt, ...)
{
    va_list ap;
    char *p;

    va_start(ap, fmt);
    p = lc_arena_vprintf(a, fmt, ap);
    va_end(ap);
    return p;
}

void lc_arena_free(lc_arena *a)
{
    lc_arena_block *b = a->head;

    while (b) {
        lc_arena_block *next = b->next;
        free(b);
        b = next;
    }
    a->head = NULL;
}

void *lc_grow(void *p, size_t *cap, size_t need, size_t elem)
{
    size_t ncap;
    void *np;

    if (need <= *cap && p)
        return p;
    ncap = *cap ? *cap : 8;
    while (ncap < need) {
        if (ncap > SIZE_MAX / 2)
            return NULL;
        ncap *= 2;
    }
    if (elem && ncap > SIZE_MAX / elem)
        return NULL;
    np = realloc(p, ncap * elem != 0 ? ncap * elem : 1);
    if (!np)
        return NULL;
    *cap = ncap;
    return np;
}
