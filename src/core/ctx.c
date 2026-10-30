#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "internal.h"

const char *lc_version(void)
{
    return LC_VERSION;
}

void lc_options_init(lc_options *opt)
{
    memset(opt, 0, sizeof *opt);
}

lc_ctx *lc_open(const lc_options *opt)
{
    lc_ctx *ctx = calloc(1, sizeof *ctx);

    if (!ctx)
        return NULL;
    lc_str_init(&ctx->err);
    if (opt && ((opt->problems_dir &&
                 !(ctx->opt_problems_dir =
                       lc_arena_strdup(&ctx->arena, opt->problems_dir))) ||
                (opt->workspace_dir &&
                 !(ctx->opt_workspace_dir =
                       lc_arena_strdup(&ctx->arena, opt->workspace_dir))) ||
                (opt->cache_dir &&
                 !(ctx->opt_cache_dir =
                       lc_arena_strdup(&ctx->arena, opt->cache_dir))))) {
        lc_close(ctx);
        return NULL;
    }
    return ctx;
}

const char *lc_cache_dir(lc_ctx *ctx)
{
    char *base;

    if (ctx->cache_dir)
        return ctx->cache_dir;
    if (ctx->opt_cache_dir) {
        /* Programs run with their build directory as the current one, so
         * every path into the cache must be absolute. */
        if (ctx->opt_cache_dir[0] == '/') {
            ctx->cache_dir = ctx->opt_cache_dir;
        } else {
            char *cwd = getcwd(NULL, 0);

            if (!cwd) {
                lc_set_errno(ctx, LC_ERR_IO, errno, "cannot use the cache "
                             "directory '%s'", ctx->opt_cache_dir);
                return NULL;
            }
            ctx->cache_dir = lc_path_join(&ctx->arena, cwd,
                                          ctx->opt_cache_dir);
            free(cwd);
            if (!ctx->cache_dir)
                lc_oom(ctx);
        }
        return ctx->cache_dir;
    }
    base = lc_xdg_dir(&ctx->arena, "XDG_CACHE_HOME", ".cache");
    if (!base) {
        if (lc_getenv("HOME"))
            lc_oom(ctx);
        else
            lc_set_error(ctx, LC_ERR_NOTFOUND, "no cache directory: set "
                         "XDG_CACHE_HOME or HOME");
        return NULL;
    }
    ctx->cache_dir = lc_path_join(&ctx->arena, base, "lcclone");
    if (!ctx->cache_dir)
        lc_oom(ctx);
    return ctx->cache_dir;
}

void lc_close(lc_ctx *ctx)
{
    if (!ctx)
        return;
    lc_str_free(&ctx->err);
    free(ctx->warnings);
    lc_arena_free(&ctx->arena);
    free(ctx);
}

const char *lc_problems_dir(const lc_ctx *ctx)
{
    return ctx->problems_dir;
}

const char *lc_last_error(const lc_ctx *ctx)
{
    if (ctx->err_nomem)
        return "out of memory";
    return lc_str_cstr(&ctx->err);
}

static int set_message(lc_ctx *ctx, int status, int errnum,
                       const char *fmt, va_list ap)
{
    /* Format into a fresh buffer first: the arguments may point into the
     * current message. */
    lc_str msg = LC_STR_INIT;

    lc_str_vprintf(&msg, fmt, ap);
    if (errnum) {
        /* Not strerror(): on macOS and the BSDs it formats into one buffer
         * shared by the whole process, so two contexts on two threads
         * would race on it. This file never defines _GNU_SOURCE, so glibc
         * declares the POSIX strerror_r that returns an int. */
        char ebuf[256];

        if (strerror_r(errnum, ebuf, sizeof ebuf) != 0)
            snprintf(ebuf, sizeof ebuf, "error %d", errnum);
        lc_str_puts(&msg, ": ");
        lc_str_puts(&msg, ebuf);
    }
    if (msg.failed) {
        lc_str_free(&msg);
        ctx->err_nomem = 1;
        return status;
    }
    lc_str_free(&ctx->err);
    ctx->err = msg;
    ctx->err_nomem = 0;
    return status;
}

int lc_set_error(lc_ctx *ctx, int status, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    set_message(ctx, status, 0, fmt, ap);
    va_end(ap);
    return status;
}

int lc_set_errno(lc_ctx *ctx, int status, int errnum, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    set_message(ctx, status, errnum, fmt, ap);
    va_end(ap);
    return status;
}

int lc_oom(lc_ctx *ctx)
{
    ctx->err_nomem = 1;
    return LC_ERR_NOMEM;
}

int lc_add_warning(lc_ctx *ctx, const char *fmt, ...)
{
    va_list ap;
    char *msg;
    char **tmp;

    va_start(ap, fmt);
    msg = lc_arena_vprintf(&ctx->arena, fmt, ap);
    va_end(ap);
    if (!msg)
        return lc_oom(ctx);
    tmp = lc_grow(ctx->warnings, &ctx->capwarnings, ctx->nwarnings + 1,
                  sizeof *ctx->warnings);
    if (!tmp)
        return lc_oom(ctx);
    ctx->warnings = tmp;
    ctx->warnings[ctx->nwarnings++] = msg;
    return LC_OK;
}

size_t lc_warning_count(const lc_ctx *ctx)
{
    return ctx->nwarnings;
}

const char *lc_warning_at(const lc_ctx *ctx, size_t i)
{
    return i < ctx->nwarnings ? ctx->warnings[i] : NULL;
}

const char *lc_difficulty_name(lc_difficulty d)
{
    switch (d) {
    case LC_EASY:
        return "easy";
    case LC_MEDIUM:
        return "medium";
    case LC_HARD:
        return "hard";
    default:
        return "unknown";
    }
}

static int ascii_ieq(const char *a, const char *b)
{
    for (; *a && *b; a++, b++) {
        int ca = (unsigned char)*a, cb = (unsigned char)*b;
        if (ca >= 'A' && ca <= 'Z')
            ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z')
            cb += 'a' - 'A';
        if (ca != cb)
            return 0;
    }
    return *a == *b;
}

lc_difficulty lc_difficulty_from_name(const char *s)
{
    if (!s)
        return LC_DIFF_UNKNOWN;
    if (ascii_ieq(s, "easy"))
        return LC_EASY;
    if (ascii_ieq(s, "medium"))
        return LC_MEDIUM;
    if (ascii_ieq(s, "hard"))
        return LC_HARD;
    return LC_DIFF_UNKNOWN;
}
