#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * build.c: building a solution with its harness (docs/PLAN.md section
 * 5.6, docs/judging.md "Building").
 *
 * Every build is one source file made of the prelude, the solution after
 * a `#line 1 "<its path>"` (so the compiler's messages point at the
 * solution's own lines), the runtime and the generated harness. It is
 * compiled in a directory of the cache named after the problem, the
 * language and the standard, into prog-<key>, where the key hashes
 * everything that goes into the build: that file, the flags and the
 * compiler's identity. A build whose program is already there is not
 * redone. Concurrent builds never clash: each compiles into names of its
 * own and renames the result into place.
 */

#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "internal.h"

/* Programs of other builds are removed once they are this old, so that
 * one still being judged by another lc is left alone. Using a program
 * makes it new again. */
#define KEEP_OLD_SEC 600
/* Longest a compile may take, and the most output kept from it. */
#define COMPILE_MS (5 * 60 * 1000)
#define COMPILE_OUTPUT_MAX (4 * LC_MIB)
/* Largest solution file lc reads. */
#define MAX_SOURCE (16 * LC_MIB)

const lc_rt_file *lc_rt_file_get(const char *name)
{
    size_t i;

    for (i = 0; i < lc_rt_nfiles; i++)
        if (strcmp(lc_rt_files[i].name, name) == 0)
            return &lc_rt_files[i];
    return NULL;
}

/* ------------------------------------------------------------------ */
/* The translation unit                                                */
/* ------------------------------------------------------------------ */

/* 64-bit FNV-1a. */
static uint64_t fnv(uint64_t h, const void *p, size_t n)
{
    const unsigned char *b = p;
    size_t i;

    for (i = 0; i < n; i++) {
        h ^= b[i];
        h *= 0x100000001b3ull;
    }
    return h;
}

/* #line 1 "file", with '"' and '\' escaped. */
static void put_line(lc_str *s, const char *file)
{
    lc_str_puts(s, "#line 1 \"");
    for (; *file; file++) {
        if (*file == '"' || *file == '\\')
            lc_str_putc(s, '\\');
        lc_str_putc(s, *file);
    }
    lc_str_puts(s, "\"\n");
}

static int put_rt(lc_ctx *ctx, lc_str *s, const char *name, const char *as)
{
    const lc_rt_file *f = lc_rt_file_get(name);

    if (!f)
        return lc_set_error(ctx, LC_ERR_INVALID, "internal error: no "
                            "runtime file '%s'", name);
    put_line(s, as);
    lc_str_append(s, f->text, f->len);
    return LC_OK;
}

static int make_unit(lc_ctx *ctx, const lc_build_req *req, const lc_str *src,
                     lc_str *tu)
{
    int c = req->lang->id == LC_LANG_C, rc;

    if (strchr(req->src, '\n'))
        return lc_set_error(ctx, LC_ERR_INVALID, "cannot build '%s': its "
                            "path has a newline in it", req->src);
    lc_str_printf(tu, "/* Built by lc: problem %d (%s), %s. */\n",
                  req->p->id, req->p->slug,
                  req->reference ? "reference solution" : "solution");
    lc_codegen_defines(req->p, tu);
    rc = put_rt(ctx, tu, c ? "c/lc_prelude.h" : "cpp/lc_prelude.hpp",
                c ? "lc_prelude.h" : "lc_prelude.hpp");
    if (rc != LC_OK)
        return rc;
    put_line(tu, req->src);
    lc_str_append(tu, lc_str_cstr(src), src->len);
    if (src->len > 0 && src->data[src->len - 1] != '\n')
        lc_str_putc(tu, '\n');
    rc = put_rt(ctx, tu, "common/lc_rt_core.h", "lc_rt_core.h");
    if (rc == LC_OK)
        rc = put_rt(ctx, tu, c ? "c/lc_rt.c" : "cpp/lc_rt.cpp",
                    c ? "lc_rt.c" : "lc_rt.cpp");
    if (rc != LC_OK)
        return rc;
    put_line(tu, LC_HARNESS_FILE);
    rc = lc_codegen_harness(ctx, req->p, req->lang, tu);
    if (rc != LC_OK)
        return rc;
    return tu->failed ? lc_oom(ctx) : LC_OK;
}

/* The compiler flags, apart from the input and output files. */
static size_t flags(const lc_build_req *req, const char *std_arg,
                    const char **out)
{
    size_t n = 0;

    out[n++] = std_arg;
    out[n++] = "-O2";
    if (req->sanitize) {
        out[n++] = "-g";
        out[n++] = "-fsanitize=address,undefined";
        /* Undefined behaviour stops the program: a Runtime Error. */
        out[n++] = "-fno-sanitize-recover=all";
        out[n++] = "-fno-omit-frame-pointer";
        if (req->lang->id == LC_LANG_CPP) {
            /* Bounds checks in the standard library: libstdc++'s, then
             * libc++'s (each ignores the other's). */
            out[n++] = "-D_GLIBCXX_ASSERTIONS";
            out[n++] = "-D_LIBCPP_HARDENING_MODE="
                       "_LIBCPP_HARDENING_MODE_EXTENSIVE";
        }
    }
    return n;
}

#define MAX_FLAGS 10

/* ------------------------------------------------------------------ */
/* Diagnostics                                                         */
/* ------------------------------------------------------------------ */

static const char *const severities[] = {"fatal error", "error", "warning",
                                         "note"};

/* If p[0..*n) ends in ":<digits>", removes that and returns the number;
 * else returns -1. */
static long strip_number(const char *p, size_t *n)
{
    size_t e = *n, b = e, i;
    long v = 0;

    while (b > 0 && p[b - 1] >= '0' && p[b - 1] <= '9')
        b--;
    if (b == e || b == 0 || p[b - 1] != ':' || e - b > 9)
        return -1;
    for (i = b; i < e; i++)
        v = v * 10 + (p[i] - '0');
    *n = b - 1;
    return v;
}

int lc_parse_diags(lc_arena *a, const char *text, size_t len,
                   const char *source, lc_diag **out, size_t *count)
{
    lc_diag *d = NULL, *tmp;
    size_t n = 0, cap = 0;
    const char *p = text, *end = text + len;

    *out = NULL;
    *count = 0;
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *le = nl ? nl : end, *m = NULL;
        size_t k, slen = 0, plen;
        long a1, a2;
        lc_diag dg;

        /* The first ": <severity>: " on the line, whichever it is. */
        memset(&dg, 0, sizeof dg);
        for (k = 0; k < sizeof severities / sizeof severities[0]; k++) {
            size_t sl = strlen(severities[k]);
            const char *q;

            for (q = p; q + sl + 4 <= le && (!m || q < m); q++) {
                if (q[0] == ':' && q[1] == ' ' &&
                    memcmp(q + 2, severities[k], sl) == 0 &&
                    q[2 + sl] == ':' && q[3 + sl] == ' ') {
                    m = q;
                    slen = sl;
                    dg.severity = k == 0 ? "error" : severities[k];
                    break;
                }
            }
        }
        if (m && m > p) {
            plen = (size_t)(m - p);
            a2 = strip_number(p, &plen);
            a1 = a2 >= 0 ? strip_number(p, &plen) : -1;
            if (a1 < 0) {
                a1 = a2;
                a2 = -1;
            }
            if (plen > 0) {
                dg.file = lc_arena_strndup(a, p, plen);
                dg.line = a1 > 0 ? (int)a1 : 0;
                dg.column = a2 > 0 ? (int)a2 : 0;
                dg.message = lc_arena_strndup(a, m + 4 + slen,
                                              (size_t)(le - m - 4 - slen));
                if (!dg.file || !dg.message) {
                    free(d);
                    return LC_ERR_NOMEM;
                }
                dg.in_solution = source && strcmp(dg.file, source) == 0;
                tmp = lc_grow(d, &cap, n + 1, sizeof *d);
                if (!tmp) {
                    free(d);
                    return LC_ERR_NOMEM;
                }
                d = tmp;
                d[n++] = dg;
            }
        }
        p = nl ? nl + 1 : end;
    }
    if (n) {
        *out = lc_arena_array(a, n, sizeof **out);
        if (!*out) {
            free(d);
            return LC_ERR_NOMEM;
        }
        memcpy(*out, d, n * sizeof *d);
    }
    free(d);
    *count = n;
    return LC_OK;
}

/* ------------------------------------------------------------------ */
/* Building                                                            */
/* ------------------------------------------------------------------ */

int lc_build_init(lc_ctx *ctx, const lc_build_req *req, lc_arena *a,
                  lc_build *b)
{
    memset(b, 0, sizeof *b);
    b->reference = req->reference;
    b->lang = req->lang->name;
    b->std = req->std;
    b->sanitize = req->sanitize;
    b->source = lc_arena_strdup(a, req->src);
    b->compiler = lc_arena_strdup(a, req->cc->cmd);
    b->output = "";
    if (!b->source || !b->compiler)
        return lc_oom(ctx);
    return LC_OK;
}

/* Removes the programs of earlier builds from `dir` that are older than
 * KEEP_OLD_SEC, and the leftovers of builds that were interrupted. */
static void remove_old(lc_ctx *ctx, const char *dir, const char *keep)
{
    lc_arena a = {NULL};
    char **names;
    size_t n, i;
    time_t now = time(NULL);

    if (lc_list_dir(ctx, dir, &a, &names, &n) == LC_OK) {
        for (i = 0; i < n; i++) {
            struct stat st;
            char *path;

            if ((strncmp(names[i], "prog-", 5) != 0 &&
                 strncmp(names[i], "main-", 5) != 0) ||
                strcmp(names[i], keep) == 0)
                continue;
            path = lc_path_join(&a, dir, names[i]);
            if (path && stat(path, &st) == 0 &&
                st.st_mtime < now - KEEP_OLD_SEC)
                unlink(path);
        }
    }
    lc_arena_free(&a);
}

/* Appends a note from lc to the compiler's output. */
static int add_note(lc_arena *a, lc_build *b, const char *note)
{
    char *s = lc_arena_printf(a, "%s%s%s\n", b->output,
                              b->output_len && b->output[b->output_len - 1]
                                  != '\n' ? "\n" : "",
                              note);

    if (!s)
        return LC_ERR_NOMEM;
    b->output = s;
    b->output_len = strlen(s);
    return LC_OK;
}

int lc_build_source(lc_ctx *ctx, const lc_build_req *req, lc_arena *a,
                    lc_build *b, const char **exe, int *cancelled)
{
    lc_arena tmp = {NULL};
    lc_str src = LC_STR_INIT, tu = LC_STR_INIT, out = LC_STR_INIT;
    const char *cache, *fl[MAX_FLAGS], *args[MAX_FLAGS + 4];
    char *dir, *prog, *prog_tmp, *unit, *unit_final, *std_arg, **argv;
    char key[17];
    uint64_t h = 0xcbf29ce484222325ull;
    size_t nfl, i;
    lc_run_spec spec;
    lc_run_result r;
    int rc;

    *exe = NULL;
    *cancelled = 0;
    lc_run_result_init(&r);

    rc = lc_read_file(ctx, req->src, MAX_SOURCE, &src);
    if (rc == LC_OK)
        rc = make_unit(ctx, req, &src, &tu);
    if (rc != LC_OK)
        goto done;

    std_arg = lc_arena_printf(&tmp, "-std=%s", req->std_flag);
    if (!std_arg) {
        rc = lc_oom(ctx);
        goto done;
    }
    nfl = flags(req, std_arg, fl);
    h = fnv(h, req->cc->id, strlen(req->cc->id) + 1);
    for (i = 0; i < nfl; i++)
        h = fnv(h, fl[i], strlen(fl[i]) + 1);
    h = fnv(h, tu.data, tu.len);
    snprintf(key, sizeof key, "%016llx", (unsigned long long)h);

    cache = lc_cache_dir(ctx);
    if (!cache) {
        rc = ctx->err_nomem ? LC_ERR_NOMEM : LC_ERR_NOTFOUND;
        goto done;
    }
    dir = lc_arena_printf(a, "%s/build/%04d-%s/%s%s-%s%s", cache, req->p->id,
                          req->p->slug, req->reference ? "ref-" : "",
                          req->lang->name, req->std,
                          req->sanitize ? "-sanitize" : "");
    prog = dir ? lc_arena_printf(a, "%s/prog-%s", dir, key) : NULL;
    if (!prog) {
        rc = lc_oom(ctx);
        goto done;
    }
    rc = lc_mkdir_p(ctx, dir, 0777);
    if (rc != LC_OK)
        goto done;
    if (access(prog, X_OK) == 0) {
        /* Mark it as in use (see remove_old). */
        utimensat(AT_FDCWD, prog, NULL, 0);
        b->ok = 1;
        b->cached = 1;
        *exe = prog;
        rc = LC_OK;
        goto done;
    }

    /* Names of this build's own, renamed into place at the end. */
    ctx->tmp_counter++;
    unit = lc_arena_printf(&tmp, "%s/main-%ld-%lu.%s", dir, (long)getpid(),
                           ctx->tmp_counter, req->lang->ext);
    unit_final = lc_arena_printf(&tmp, "%s/main.%s", dir, req->lang->ext);
    prog_tmp = lc_arena_printf(&tmp, "%s.tmp-%ld-%lu", prog, (long)getpid(),
                               ctx->tmp_counter);
    if (!unit || !unit_final || !prog_tmp) {
        rc = lc_oom(ctx);
        goto done;
    }
    rc = lc_write_file_atomic(ctx, unit, tu.data, tu.len);
    if (rc != LC_OK)
        goto done;
    for (i = 0; i < nfl; i++)
        args[i] = fl[i];
    args[i++] = "-o";
    args[i++] = prog_tmp;
    args[i++] = unit;
    if (req->lang->id == LC_LANG_C)
        args[i++] = "-lm";
    argv = lc_compiler_argv(&tmp, req->cc, args, i);
    if (!argv) {
        rc = lc_oom(ctx);
        goto done;
    }

    memset(&spec, 0, sizeof spec);
    spec.path = argv[0];
    spec.argv = argv;
    spec.cwd = dir;
    spec.wall_ms = COMPILE_MS;
    spec.stdout_max = COMPILE_OUTPUT_MAX;
    spec.stderr_max = COMPILE_OUTPUT_MAX;
    spec.cancel = req->cancel;
    rc = lc_run(ctx, &spec, &r);
    if (rc != LC_OK)
        goto cleanup;
    b->time_ms = r.wall_ms;
    if (r.cancelled) {
        *cancelled = 1;
        goto cleanup;
    }

    lc_str_append(&out, lc_str_cstr(&r.out), r.out.len);
    lc_str_append(&out, lc_str_cstr(&r.err), r.err.len);
    if (out.failed) {
        rc = lc_oom(ctx);
        goto cleanup;
    }
    b->output = lc_arena_strndup(a, lc_str_cstr(&out), out.len);
    if (!b->output) {
        rc = lc_oom(ctx);
        goto cleanup;
    }
    b->output_len = out.len;
    b->ok = r.exited && r.exit_code == 0 && !r.overflow &&
            access(prog_tmp, X_OK) == 0;
    if (r.timed_out)
        rc = add_note(a, b, "lc: the compiler was still running after "
                      "5 minutes, and was stopped");
    else if (r.overflow)
        rc = add_note(a, b, "lc: the compiler printed more than 4 MiB, and "
                      "was stopped");
    else if (r.signal)
        rc = add_note(a, b, "lc: the compiler was killed by a signal");
    if (rc != LC_OK) {
        rc = lc_oom(ctx);
        goto cleanup;
    }
    if (lc_parse_diags(a, b->output, b->output_len, req->src,
                       (lc_diag **)&b->diags, &b->ndiags) != LC_OK) {
        rc = lc_oom(ctx);
        goto cleanup;
    }
    if (b->ok) {
        if (rename(prog_tmp, prog) != 0) {
            rc = lc_set_errno(ctx, LC_ERR_IO, errno, "cannot create '%s'",
                              prog);
            goto cleanup;
        }
        remove_old(ctx, dir, prog + strlen(dir) + 1);
        *exe = prog;
    }

cleanup:
    unlink(prog_tmp);
    /* Keep the last unit as main.c(pp), for looking at what was built. */
    if (rename(unit, unit_final) != 0)
        unlink(unit);
done:
    lc_run_result_free(&r);
    lc_str_free(&src);
    lc_str_free(&tu);
    lc_str_free(&out);
    lc_arena_free(&tmp);
    return rc;
}
