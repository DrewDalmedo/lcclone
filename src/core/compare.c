#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * compare.c: checks a result against the expected one under a `compare:`
 * mode (docs/problem-format.md, "compare").
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

/* Equal values, with doubles allowed to differ by up to tol when tol >= 0. */
static int equal(const lc_value *a, const lc_value *b, double tol)
{
    size_t i;

    if (a->kind != b->kind)
        return 0;
    switch (a->kind) {
    case LC_V_NULL:
        return 1;
    case LC_V_BOOL:
        return a->u.b == b->u.b;
    case LC_V_INT:
        return a->u.i == b->u.i;
    case LC_V_FLOAT:
        if (tol >= 0) {
            double d = a->u.f - b->u.f;
            if (d < 0)
                d = -d;
            return d <= tol; /* false for NaN */
        }
        return a->u.f == b->u.f;
    case LC_V_STRING:
        return a->len == b->len && memcmp(a->u.s, b->u.s, a->len) == 0;
    case LC_V_ARRAY:
        if (a->len != b->len)
            return 0;
        for (i = 0; i < a->len; i++)
            if (!equal(&a->u.items[i], &b->u.items[i], tol))
                return 0;
        return 1;
    }
    return 0;
}

/* A total order on values, for sorting: by kind, then by value. NaN sorts
 * after every other double. */
static int order(const lc_value *a, const lc_value *b)
{
    size_t i, n;
    int c;

    if (a->kind != b->kind)
        return a->kind < b->kind ? -1 : 1;
    switch (a->kind) {
    case LC_V_NULL:
        return 0;
    case LC_V_BOOL:
        return (a->u.b > b->u.b) - (a->u.b < b->u.b);
    case LC_V_INT:
        return (a->u.i > b->u.i) - (a->u.i < b->u.i);
    case LC_V_FLOAT: {
        int na = isnan(a->u.f) != 0, nb = isnan(b->u.f) != 0;
        if (na || nb)
            return na - nb;
        return (a->u.f > b->u.f) - (a->u.f < b->u.f);
    }
    case LC_V_STRING:
        n = a->len < b->len ? a->len : b->len;
        c = memcmp(a->u.s, b->u.s, n);
        if (c)
            return c < 0 ? -1 : 1;
        return (a->len > b->len) - (a->len < b->len);
    case LC_V_ARRAY:
        n = a->len < b->len ? a->len : b->len;
        for (i = 0; i < n; i++) {
            c = order(&a->u.items[i], &b->u.items[i]);
            if (c)
                return c;
        }
        return (a->len > b->len) - (a->len < b->len);
    }
    return 0;
}

static int cmp_values(const void *a, const void *b)
{
    return order(a, b);
}

/*
 * A copy of `v` in which the arrays at the levels in `levels` (level is
 * v's own) are sorted, or v itself when nothing at or below it needs
 * sorting. NULL when out of memory.
 */
static const lc_value *sorted(lc_arena *a, const lc_value *v, int level,
                              unsigned levels)
{
    lc_value *copy, *items;
    size_t i;

    if (v->kind != LC_V_ARRAY || level >= LC_MAX_DIMS ||
        (levels >> level) == 0)
        return v;
    copy = lc_arena_alloc(a, sizeof *copy);
    items = lc_arena_array(a, v->len ? v->len : 1, sizeof *items);
    if (!copy || !items)
        return NULL;
    for (i = 0; i < v->len; i++) {
        const lc_value *s = sorted(a, &v->u.items[i], level + 1, levels);
        if (!s)
            return NULL;
        items[i] = *s;
    }
    if (levels & (1u << level))
        qsort(items, v->len, sizeof *items, cmp_values);
    *copy = *v;
    copy->u.items = items;
    return copy;
}

/* The status to return after setting the last error (see cases.c). */
static int error_status(const lc_ctx *ctx, int rc)
{
    return ctx->err_nomem ? LC_ERR_NOMEM : rc;
}

int lc_value_match(lc_ctx *ctx, const lc_compare *cmp,
                   const lc_value *expected, const lc_value *got, int *match)
{
    lc_arena scratch;
    const lc_value *se, *sg;

    if (!cmp || !expected || !got || !match)
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "lc_value_match: bad "
                                              "arguments"));
    *match = 0;
    switch (cmp->mode) {
    case LC_CMP_EXACT:
        *match = equal(expected, got, -1);
        return LC_OK;
    case LC_CMP_FLOAT:
        *match = equal(expected, got, cmp->tolerance);
        return LC_OK;
    case LC_CMP_UNORDERED:
        memset(&scratch, 0, sizeof scratch);
        se = sorted(&scratch, expected, 0, cmp->levels);
        sg = se ? sorted(&scratch, got, 0, cmp->levels) : NULL;
        if (!sg) {
            lc_arena_free(&scratch);
            return lc_oom(ctx);
        }
        *match = equal(se, sg, -1);
        lc_arena_free(&scratch);
        return LC_OK;
    case LC_CMP_CHECKER:
        return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                              "compare: checker needs the "
                                              "pack's checker, which "
                                              "lc_value_match can't run"));
    }
    return error_status(ctx, lc_set_error(ctx, LC_ERR_INVALID,
                                          "lc_value_match: unknown mode"));
}
