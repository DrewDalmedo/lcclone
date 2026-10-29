/* STUB: replaced by the real pack loader. */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stddef.h>

#include "internal.h"

int lc_load(lc_ctx *ctx)
{
    return lc_set_error(ctx, LC_ERR_NOTFOUND, "pack loader not implemented");
}

size_t lc_problem_count(const lc_ctx *ctx)
{
    return ctx->nproblems;
}

const lc_problem *lc_problem_at(const lc_ctx *ctx, size_t i)
{
    return i < ctx->nproblems ? &ctx->problems[i] : NULL;
}

const lc_problem *lc_problem_find(lc_ctx *ctx, const char *key)
{
    lc_set_error(ctx, LC_ERR_NOTFOUND, "no problem matches '%s'", key);
    return NULL;
}

const char *lc_problem_field(const lc_problem *p, const char *key)
{
    (void)p;
    (void)key;
    return NULL;
}

int lc_problem_has_tag(const lc_problem *p, const char *tag)
{
    (void)p;
    (void)tag;
    return 0;
}
