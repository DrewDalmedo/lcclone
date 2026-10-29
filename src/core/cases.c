/* STUB: replaced by the real tests.txt reader. */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>

#include "internal.h"

int lc_parse_cases(lc_ctx *ctx, const char *text, size_t len,
                   const char *filename, lc_cases **out)
{
    (void)text;
    (void)len;
    (void)filename;
    *out = calloc(1, sizeof **out);
    return *out ? LC_OK : lc_oom(ctx);
}

int lc_load_cases(lc_ctx *ctx, const lc_problem *p, lc_cases **out)
{
    (void)p;
    return lc_parse_cases(ctx, "", 0, "tests.txt", out);
}

void lc_cases_free(lc_cases *cases)
{
    free(cases);
}
