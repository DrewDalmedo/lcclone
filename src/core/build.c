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
 * compiler's identity. Next to the program, prog-<key>.log keeps what the
 * compiler printed, so a build whose program is already there is not
 * redone and still has its warnings. Concurrent builds never clash: each
 * compiles into names of its own and renames the results into place.
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

/*
 * Apple's compilers, given -g (as --sanitize builds are), run dsymutil
 * after linking, which writes the debug info next to the program as a
 * directory, <program>.dSYM. atos, which turns the sanitizers' stack
 * traces into file:line there, looks for it under the program's name, so
 * it is renamed along with the program. Other systems keep the debug info
 * in the program itself.
 */
#ifdef __APPLE__
#define DSYM 1
#else
#define DSYM 0
#endif

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

const char *lc_role_name(int role)
{
    switch (role) {
    case LC_ROLE_REF:
        return "reference";
    case LC_ROLE_BRUTE:
        return "brute";
    case LC_ROLE_STUB:
        return "stub";
    case LC_ROLE_GEN:
        return "generator";
    default:
        return "solution";
    }
}

/* The generator: lc_gen.h, gen.c, then its main(). */
static int make_gen_unit(lc_ctx *ctx, const lc_build_req *req,
                         const lc_str *src, lc_str *tu)
{
    int rc;

    lc_str_printf(tu, "/* Built by lc: problem %d (%s), generator. */\n",
                  req->p->id, req->p->slug);
    rc = put_rt(ctx, tu, "gen/lc_gen.h", "lc_gen.h");
    if (rc != LC_OK)
        return rc;
    put_line(tu, req->src);
    lc_str_append(tu, lc_str_cstr(src), src->len);
    if (src->len > 0 && src->data[src->len - 1] != '\n')
        lc_str_putc(tu, '\n');
    rc = put_rt(ctx, tu, "gen/lc_gen_main.c", "lc_gen_main.c");
    if (rc != LC_OK)
        return rc;
    return tu->failed ? lc_oom(ctx) : LC_OK;
}

static int make_unit(lc_ctx *ctx, const lc_build_req *req, const lc_str *src,
                     lc_str *tu)
{
    int c = req->lang->id == LC_LANG_C, rc;

    if (strchr(req->src, '\n'))
        return lc_set_error(ctx, LC_ERR_INVALID, "cannot build '%s': its "
                            "path has a newline in it", req->src);
    if (req->role == LC_ROLE_GEN)
        return make_gen_unit(ctx, req, src, tu);
    lc_str_printf(tu, "/* Built by lc: problem %d (%s), %s. */\n",
                  req->p->id, req->p->slug,
                  req->role == LC_ROLE_REF ? "reference solution"
                                           : lc_role_name(req->role));
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
    /* uthash.h is next to the unit, also for a solution that includes it
     * with <> (see lc_build_source). */
    if (req->lang->id == LC_LANG_C)
        out[n++] = "-I.";
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

#define MAX_FLAGS 11

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
    b->reference = req->role == LC_ROLE_REF;
    b->role = lc_role_name(req->role);
    b->lang = req->lang->name;
    b->std = req->std;
    b->sanitize = req->sanitize;
    b->source = lc_arena_strdup(a, req->src);
    b->compiler = lc_arena_strdup(a, req->cc->cmd);
    b->output = "";
    if (!b->source || !b->compiler)
        return lc_oom(ctx);
    /* The generator has no harness, so nothing of it is called. */
    if (req->role != LC_ROLE_GEN && req->p->sig) {
        lc_str proto = LC_STR_INIT;
        int rc = lc_prototype(ctx, req->p, req->lang, &proto);

        if (rc == LC_OK) {
            b->prototype = lc_arena_strndup(a, lc_str_cstr(&proto),
                                            proto.len);
            if (!b->prototype)
                rc = lc_oom(ctx);
        }
        lc_str_free(&proto);
        if (rc != LC_OK)
            return rc;
    }
    return LC_OK;
}

/* Marks a file as in use (see remove_old). */
static void touch(const char *path)
{
    utimensat(AT_FDCWD, path, NULL, 0);
}

/* 1 if `name` is the program `keep` or one of the files kept with it. */
static int kept_with(const char *name, const char *keep)
{
    size_t n = strlen(keep);

    return strncmp(name, keep, n) == 0 &&
           (name[n] == '\0' || strcmp(name + n, ".log") == 0 ||
            strcmp(name + n, ".dSYM") == 0);
}

/* Removes the programs of earlier builds from `dir` that are older than
 * KEEP_OLD_SEC, with their .log and .dSYM, and the leftovers of builds
 * that were interrupted. */
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
                kept_with(names[i], keep))
                continue;
            path = lc_path_join(&a, dir, names[i]);
            if (path && lstat(path, &st) == 0 &&
                st.st_mtime < now - KEEP_OLD_SEC)
                lc_remove_tree(path);
        }
    }
    lc_arena_free(&a);
}

/* Most of a .log that is read back: all the compiler output kept. */
#define LOG_MAX (2 * COMPILE_OUTPUT_MAX)

/*
 * Reuses the program of an earlier build of the same input, with the
 * compiler output kept in its .log. LC_ERR_NOTFOUND when there is none to
 * reuse: no program, or one without its .log (built by an older lc, or
 * left alone after its .log was removed), which is then built again.
 */
static int reuse(lc_ctx *ctx, const lc_build_req *req, lc_arena *a,
                 lc_build *b, const char *prog, const char *logfile,
                 const char *dsym)
{
    lc_str out = LC_STR_INIT;
    int rc;

    if (access(prog, X_OK) != 0 || access(logfile, F_OK) != 0)
        return LC_ERR_NOTFOUND;
    rc = lc_read_file(ctx, logfile, LOG_MAX, &out);
    if (rc == LC_OK) {
        b->output = lc_arena_strndup(a, lc_str_cstr(&out), out.len);
        b->output_len = out.len;
        /* The same messages as the first time: the key covers the
         * solution's path, which decides `in_solution`. */
        if (!b->output ||
            lc_parse_diags(a, b->output, b->output_len, req->src,
                           (lc_diag **)&b->diags, &b->ndiags) != LC_OK)
            rc = lc_oom(ctx);
    } else if (rc == LC_ERR_NOMEM || ctx->err_nomem) {
        rc = lc_oom(ctx);
    } else {
        rc = LC_ERR_NOTFOUND; /* unreadable: built again */
    }
    lc_str_free(&out);
    if (rc != LC_OK) {
        b->output = "";
        b->output_len = 0;
        b->diags = NULL;
        b->ndiags = 0;
        return rc;
    }
    touch(prog);
    touch(logfile);
    if (dsym)
        touch(dsym);
    b->ok = 1;
    b->cached = 1;
    return LC_OK;
}

/* Moves a new program's .dSYM to its final name, replacing one left there
 * by an earlier build. Without it a stack trace has no file:line, but the
 * program runs, so failing here is no error. */
static void place_dsym(const char *from, const char *to)
{
    if (!lc_is_dir(from))
        return;
    if (lc_remove_tree(to) != 0 || rename(from, to) != 0)
        lc_remove_tree(from);
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
    const lc_rt_file *uthash;
    const char *cache, *extra, *fl[MAX_FLAGS], *args[MAX_FLAGS + 4];
    char *dir, *prog, *prog_tmp, *unit, *unit_final, *std_arg, **argv;
    char *logfile, *dsym = NULL, *dsym_tmp = NULL;
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
    /* The config's flags are for your solutions, not lc's own sources. */
    extra = req->role == LC_ROLE_SOLUTION ? req->cc->flags : NULL;
    h = fnv(h, req->cc->id, strlen(req->cc->id) + 1);
    for (i = 0; i < nfl; i++)
        h = fnv(h, fl[i], strlen(fl[i]) + 1);
    if (extra)
        h = fnv(h, extra, strlen(extra) + 1);
    h = fnv(h, tu.data, tu.len);
    uthash = req->lang->id == LC_LANG_C ? lc_rt_file_get("c/uthash.h")
                                        : NULL;
    if (uthash)
        h = fnv(h, uthash->text, uthash->len);
    snprintf(key, sizeof key, "%016llx", (unsigned long long)h);

    cache = lc_cache_dir(ctx);
    if (!cache) {
        rc = ctx->err_nomem ? LC_ERR_NOMEM : LC_ERR_NOTFOUND;
        goto done;
    }
    dir = lc_arena_printf(a, "%s/build/%04d-%s/%s%s%s-%s%s", cache,
                          req->p->id, req->p->slug,
                          req->role == LC_ROLE_SOLUTION
                              ? ""
                              : req->role == LC_ROLE_REF ? "ref"
                                                         : lc_role_name(req->role),
                          req->role == LC_ROLE_SOLUTION ? "" : "-",
                          req->lang->name, req->std,
                          req->sanitize ? "-sanitize" : "");
    prog = dir ? lc_arena_printf(a, "%s/prog-%s", dir, key) : NULL;
    logfile = prog ? lc_arena_printf(&tmp, "%s.log", prog) : NULL;
    if (DSYM && logfile)
        dsym = lc_arena_printf(&tmp, "%s.dSYM", prog);
    if (!logfile || (DSYM && !dsym)) {
        rc = lc_oom(ctx);
        goto done;
    }
    rc = lc_mkdir_p(ctx, dir, 0777);
    if (rc != LC_OK)
        goto done;
    rc = reuse(ctx, req, a, b, prog, logfile, dsym);
    if (rc == LC_OK)
        *exe = prog;
    if (rc != LC_ERR_NOTFOUND)
        goto done;

    /* Names of this build's own, renamed into place at the end. */
    ctx->tmp_counter++;
    unit = lc_arena_printf(&tmp, "%s/main-%ld-%lu.%s", dir, (long)getpid(),
                           ctx->tmp_counter, req->lang->ext);
    unit_final = lc_arena_printf(&tmp, "%s/main.%s", dir, req->lang->ext);
    prog_tmp = lc_arena_printf(&tmp, "%s.tmp-%ld-%lu", prog, (long)getpid(),
                               ctx->tmp_counter);
    if (DSYM && prog_tmp)
        dsym_tmp = lc_arena_printf(&tmp, "%s.dSYM", prog_tmp);
    if (!unit || !unit_final || !prog_tmp || (DSYM && !dsym_tmp)) {
        rc = lc_oom(ctx);
        goto done;
    }
    rc = lc_write_file_atomic(ctx, unit, tu.data, tu.len);
    /* The prelude's #include "uthash.h" finds it next to the unit. */
    if (rc == LC_OK && uthash) {
        char *path = lc_arena_printf(&tmp, "%s/uthash.h", dir);

        rc = path ? lc_write_file_atomic(ctx, path, uthash->text,
                                         uthash->len)
                  : lc_oom(ctx);
    }
    if (rc != LC_OK)
        goto done;
    for (i = 0; i < nfl; i++)
        args[i] = fl[i];
    args[i++] = "-o";
    args[i++] = prog_tmp;
    args[i++] = unit;
    if (req->lang->id == LC_LANG_C)
        args[i++] = "-lm";
    argv = lc_compiler_argv(&tmp, req->cc, args, i, extra);
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
        /* The .log and the .dSYM go first: a program in place has them
         * (see reuse). */
        rc = lc_write_file_atomic(ctx, logfile, b->output, b->output_len);
        if (rc != LC_OK)
            goto cleanup;
        if (dsym_tmp)
            place_dsym(dsym_tmp, dsym);
        if (rename(prog_tmp, prog) != 0) {
            rc = lc_set_errno(ctx, LC_ERR_IO, errno, "cannot create '%s'",
                              prog);
            unlink(logfile);
            if (dsym)
                lc_remove_tree(dsym);
            goto cleanup;
        }
        remove_old(ctx, dir, prog + strlen(dir) + 1);
        *exe = prog;
    }

cleanup:
    unlink(prog_tmp);
    if (dsym_tmp)
        lc_remove_tree(dsym_tmp);
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
