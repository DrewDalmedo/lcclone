#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * full.c: full-size cases (docs/PLAN.md section 6.4). A case at the
 * limits of a problem's constraints can be megabytes of text, so
 * tests.txt keeps only its seed and the hash of its input, "@full SEED
 * HASH", and the pack's gen.c makes the input again when it is judged.
 * The hash makes sure it is the same input on every system. A result too
 * long to keep in tests.txt is worked out by the reference solution and
 * kept in the cache.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

/* ------------------------------------------------------------------ */
/* Running gen                                                         */
/* ------------------------------------------------------------------ */

int lc_gen_run(lc_ctx *ctx, const char *gen_exe, unsigned long long seed,
               int small, unsigned long long count,
               const volatile sig_atomic_t *cancel, lc_arena *a,
               lc_run_result *r)
{
    lc_run_spec spec;
    char seedbuf[24], countbuf[24];
    char *argv[5];

    snprintf(seedbuf, sizeof seedbuf, "%llu", seed);
    snprintf(countbuf, sizeof countbuf, "%llu", count);
    argv[0] = (char *)gen_exe;
    argv[1] = seedbuf;
    argv[2] = small ? "small" : "large";
    argv[3] = countbuf;
    argv[4] = NULL;
    memset(&spec, 0, sizeof spec);
    spec.path = gen_exe;
    spec.argv = argv;
    spec.cwd = lc_dir_of(a, gen_exe);
    spec.cpu_sec = LC_GEN_CPU_SEC;
    spec.wall_ms = LC_GEN_WALL_MS;
    spec.as_bytes = LC_GEN_MEMORY;
    spec.stdout_max = LC_GEN_OUTPUT_MAX;
    spec.cancel = cancel;
    if (!spec.cwd)
        return lc_oom(ctx);
    return lc_run(ctx, &spec, r);
}

int lc_gen_failed(const lc_run_result *r, lc_str *why)
{
    lc_str_clear(why);
    if (r->exited && r->exit_code == 0 && !r->overflow)
        return 0;
    if (r->overflow & LC_RUN_STDOUT)
        lc_str_puts(why, "printed more than 256 MiB");
    else if (r->overflow)
        lc_str_puts(why, "wrote more than 1 MiB on stderr");
    else if (r->timed_out)
        lc_str_printf(why, "still running after %d s", LC_GEN_WALL_MS / 1000);
    else if (r->signal)
        lc_str_printf(why, "killed by %s", lc_signal_name(r->signal)
                                               ? lc_signal_name(r->signal)
                                               : "a signal");
    else
        lc_str_printf(why, "exit status %d", r->exit_code);
    return 1;
}

/* ------------------------------------------------------------------ */
/* The hash of an input                                                */
/* ------------------------------------------------------------------ */

void lc_full_hash(const char *const *args, size_t nargs, char *out)
{
    uint64_t h = 0xcbf29ce484222325ull;
    size_t i, k;

    /* 64-bit FNV-1a over the input lines, each ended by '\n', folded to
     * 32 bits. */
    for (i = 0; i < nargs; i++) {
        for (k = 0; args[i][k]; k++) {
            h ^= (unsigned char)args[i][k];
            h *= 0x100000001b3ull;
        }
        h ^= '\n';
        h *= 0x100000001b3ull;
    }
    snprintf(out, LC_FULL_HASH_LEN + 1, "%08lx",
             (unsigned long)((h ^ (h >> 32)) & 0xffffffffu));
}

/* ------------------------------------------------------------------ */
/* Making a full-size case                                             */
/* ------------------------------------------------------------------ */

int lc_full_make(lc_ctx *ctx, const lc_problem *p, const char *gen_exe,
                 const lc_case *c, const volatile sig_atomic_t *cancel,
                 lc_cases **made, char *hash, int *mismatch, long *gen_ms,
                 int *cancelled)
{
    lc_arena a = {NULL};
    lc_run_result r;
    lc_str why = LC_STR_INIT, name = LC_STR_INIT;
    char got[LC_FULL_HASH_LEN + 1];
    const char *file = c->file;
    char *tests = NULL;
    lc_case *item;
    int rc;

    *made = NULL;
    *cancelled = 0;
    if (mismatch)
        *mismatch = 0;
    if (gen_ms)
        *gen_ms = 0;
    if (!file) {
        tests = lc_path_join(&a, p->dir, "tests.txt");
        if (!tests) {
            lc_arena_free(&a);
            return lc_oom(ctx);
        }
        file = tests;
    }
    lc_run_result_init(&r);
    rc = lc_gen_run(ctx, gen_exe, c->seed, 0, 1, cancel, &a, &r);
    if (rc != LC_OK)
        goto done;
    if (r.cancelled) {
        *cancelled = 1;
        goto done;
    }
    if (gen_ms)
        *gen_ms = r.cpu_ms;
    if (lc_gen_failed(&r, &why)) {
        const char *err = lc_str_cstr(&r.err), *nl;
        size_t n = r.err.len;

        /* The last line gen printed on stderr says why, if anything does. */
        while (n > 0 && (err[n - 1] == '\n' || err[n - 1] == '\r'))
            n--;
        nl = n ? err + n : err;
        while (nl > err && nl[-1] != '\n')
            nl--;
        rc = lc_set_error(ctx, LC_ERR_INVALID, "%s:%d: gen.c failed making "
                          "the full-size case of seed %llu (%s)%s%.*s",
                          file, c->line, c->seed, lc_str_cstr(&why),
                          n ? ": " : "", (int)(err + n - nl), nl);
        goto done;
    }
    if (lc_str_printf(&name, "the output of gen.c for '@full %llu'",
                      c->seed) != LC_OK) {
        rc = lc_oom(ctx);
        goto done;
    }
    rc = lc_parse_user_cases(ctx, lc_str_cstr(&r.out), r.out.len, name.data,
                             made);
    if (rc == LC_ERR_NOMEM)
        goto done;
    if (rc != LC_OK) {
        rc = lc_set_error(ctx, LC_ERR_INVALID, "%s:%d: %s", file, c->line,
                          lc_last_error(ctx));
        goto done;
    }
    if ((*made)->count != 1 || (*made)->items[0].full) {
        rc = lc_set_error(ctx, LC_ERR_INVALID, "%s:%d: gen.c printed %lu "
                          "cases for the full-size case of seed %llu "
                          "instead of one", file, c->line,
                          (unsigned long)(*made)->count, c->seed);
        goto done;
    }
    lc_full_hash((*made)->items[0].args, (*made)->items[0].nargs, got);
    if (hash)
        memcpy(hash, got, sizeof got);
    if (c->hash && strcmp(c->hash, got) != 0) {
        if (mismatch)
            *mismatch = 1;
        rc = lc_set_error(ctx, LC_ERR_INVALID, "%s:%d: gen.c made another "
                          "case for '@full %llu' than the one whose hash is "
                          "%s (its hash is %s), so it can't be judged here",
                          file, c->line, c->seed, c->hash, got);
        goto done;
    }
    /* The case keeps its place: where it is, its seed and hash. */
    item = (lc_case *)&(*made)->items[0];
    item->file = c->file;
    item->line = c->line;
    item->last_line = c->last_line;
    item->expected_line = c->expected_line;
    item->expected = c->expected;
    item->full = 1;
    item->seed = c->seed;
    item->hash = c->hash;
    rc = LC_OK;
done:
    if (rc != LC_OK || *cancelled) {
        lc_cases_free(*made);
        *made = NULL;
        if (ctx->err_nomem)
            rc = LC_ERR_NOMEM;
    }
    lc_run_result_free(&r);
    lc_str_free(&why);
    lc_str_free(&name);
    lc_arena_free(&a);
    return rc;
}

/* ------------------------------------------------------------------ */
/* What a full-size case's input looks like                            */
/* ------------------------------------------------------------------ */

/* Longest input of a full-size case shown as it is written. */
#define SHOW_MAX 40

int lc_full_size(lc_arena *a, const lc_type *t, const lc_value *v,
                 const char *literal, const char **out)
{
    size_t i, total = 0;
    int same = 1;

    *out = NULL;
    if (strlen(literal) <= SHOW_MAX)
        return LC_OK;
    if (v->kind == LC_V_STRING) {
        *out = lc_arena_printf(a, "%lu characters", (unsigned long)v->len);
    } else if (v->kind != LC_V_ARRAY) {
        *out = lc_arena_printf(a, "%lu bytes", (unsigned long)strlen(literal));
    } else if (t && t->dims == 0 && (t->kind == LC_T_LIST ||
                                     t->kind == LC_T_TREE ||
                                     t->kind == LC_T_NARY)) {
        /* A list or a tree: its nodes, not the nulls of its level order. */
        for (i = 0; i < v->len; i++)
            total += v->u.items[i].kind != LC_V_NULL;
        *out = lc_arena_printf(a, "%lu node%s", (unsigned long)total,
                               total == 1 ? "" : "s");
    } else if (!t || t->dims < 2) {
        *out = lc_arena_printf(a, "%lu item%s", (unsigned long)v->len,
                               v->len == 1 ? "" : "s");
    } else {
        for (i = 0; i < v->len; i++) {
            total += v->u.items[i].len;
            if (v->u.items[i].len != v->u.items[0].len)
                same = 0;
        }
        if (same && v->len)
            *out = lc_arena_printf(a, "%lu x %lu items",
                                   (unsigned long)v->len,
                                   (unsigned long)v->u.items[0].len);
        else
            *out = lc_arena_printf(a, "%lu rows, %lu items",
                                   (unsigned long)v->len,
                                   (unsigned long)total);
    }
    return *out ? LC_OK : LC_ERR_NOMEM;
}

/* ------------------------------------------------------------------ */
/* Results worked out by ref, kept in the cache                        */
/* ------------------------------------------------------------------ */

/* Largest result kept in the cache. */
#define CACHE_RESULT_MAX (64 * LC_MIB)

/* The build key of a program lc built: the <key> of its prog-<key>. */
static const char *exe_key(const char *exe)
{
    const char *base = strrchr(exe, '/');

    base = base ? base + 1 : exe;
    return strncmp(base, "prog-", 5) == 0 ? base + 5 : base;
}

/* <cache>/full/NNNN-slug/<gen key>-<ref key>-<seed>, in `a`. */
static char *cache_path(lc_ctx *ctx, const lc_problem *p, const char *gen_exe,
                        const char *ref_exe, unsigned long long seed,
                        lc_arena *a, char **dir)
{
    const char *cache = lc_cache_dir(ctx);

    if (!cache)
        return NULL;
    *dir = lc_arena_printf(a, "%s/full/%04d-%s", cache, p->id, p->slug);
    if (!*dir)
        return NULL;
    return lc_arena_printf(a, "%s/%s-%s-%llu", *dir, exe_key(gen_exe),
                           exe_key(ref_exe), seed);
}

int lc_full_cache_get(lc_ctx *ctx, const lc_problem *p, const char *gen_exe,
                      const char *ref_exe, unsigned long long seed,
                      lc_arena *a, const char **out)
{
    lc_arena tmp = {NULL};
    lc_str text = LC_STR_INIT;
    char *dir, *path;
    int rc;

    *out = NULL;
    path = cache_path(ctx, p, gen_exe, ref_exe, seed, &tmp, &dir);
    if (!path) {
        lc_arena_free(&tmp);
        return ctx->err_nomem ? LC_ERR_NOMEM : LC_OK;
    }
    rc = lc_read_file(ctx, path, CACHE_RESULT_MAX, &text);
    if (rc == LC_OK && text.len > 0 && text.data[text.len - 1] == '\n') {
        *out = lc_arena_strndup(a, text.data, text.len - 1);
        rc = *out ? LC_OK : lc_oom(ctx);
    } else {
        /* Missing, unreadable or cut short: worked out again. */
        rc = ctx->err_nomem ? LC_ERR_NOMEM : LC_OK;
    }
    lc_str_free(&text);
    lc_arena_free(&tmp);
    return rc;
}

int lc_full_cache_put(lc_ctx *ctx, const lc_problem *p, const char *gen_exe,
                      const char *ref_exe, unsigned long long seed,
                      const char *result)
{
    lc_arena tmp = {NULL};
    lc_str text = LC_STR_INIT;
    char *dir, *path;
    int rc = LC_OK;

    path = cache_path(ctx, p, gen_exe, ref_exe, seed, &tmp, &dir);
    lc_str_printf(&text, "%s\n", result);
    if (!path || text.failed) {
        rc = ctx->err_nomem || text.failed ? lc_oom(ctx) : LC_OK;
    } else if (lc_mkdir_p(ctx, dir, 0777) == LC_OK) {
        /* A cache that can't be written costs time, not results. */
        if (lc_write_file_atomic(ctx, path, text.data, text.len) != LC_OK &&
            ctx->err_nomem)
            rc = LC_ERR_NOMEM;
    } else if (ctx->err_nomem) {
        rc = LC_ERR_NOMEM;
    }
    lc_str_free(&text);
    lc_arena_free(&tmp);
    return rc;
}
