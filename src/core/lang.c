#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * lang.c: the solution languages, their compilers and their standards
 * (docs/PLAN.md section 5.6).
 *
 * A compiler is a command such as "c++" or "ccache g++", run through
 * /bin/sh as make would run it. Its first word, looked up on $PATH, says
 * whether it is installed, and that file's size and time stamp tell one
 * version of it from another, for the caches.
 *
 * The default standards (c11, c++11) are used as they are. Any other is
 * tried once with an empty file, and the answer is kept in
 * <cache>/stds, so that asking for one the compiler lacks gives a clear
 * error instead of a confusing compile failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "internal.h"

/* Longest a standard probe may take. */
#define PROBE_MS 60000

static const char *const c_stds[] = {"c99", "c11", "c17", "c23", NULL};
static const char *const cpp_stds[] = {"c++11", "c++14", "c++17", "c++20",
                                       "c++23", NULL};

/* Older spellings of the same standards, for older compilers. */
static const struct {
    const char *std;
    const char *alt;
} alt_stds[] = {
    {"c17", "c18"},     {"c23", "c2x"},     {"c++14", "c++1y"},
    {"c++17", "c++1z"}, {"c++20", "c++2a"}, {"c++23", "c++2b"},
};

static const lc_lang langs[] = {
    {LC_LANG_C, "c", "C", "c", "CC", "cc", "LC_CSTD", "c11", c_stds},
    {LC_LANG_CPP, "cpp", "C++", "cpp", "CXX", "c++", "LC_CXXSTD", "c++11",
     cpp_stds},
};

#define NLANGS (sizeof langs / sizeof langs[0])

/* Each language's keys in the config file, by lc_lang_id. */
static const struct {
    const char *compiler;
    const char *std;
    const char *flags;
} config_keys[] = {
    {"cc", "cstd", "cflags"},
    {"cxx", "cxxstd", "cxxflags"},
};

size_t lc_lang_count(void)
{
    return NLANGS;
}

const lc_lang *lc_lang_at(size_t i)
{
    return i < NLANGS ? &langs[i] : NULL;
}

const lc_lang *lc_lang_find(const char *name)
{
    if (!name)
        return NULL;
    if (strcmp(name, "c") == 0)
        return &langs[LC_LANG_C];
    if (strcmp(name, "cpp") == 0 || strcmp(name, "c++") == 0 ||
        strcmp(name, "cxx") == 0)
        return &langs[LC_LANG_CPP];
    return NULL;
}

const lc_lang *lc_lang_for_path(const char *path)
{
    const char *slash, *dot;

    if (!path)
        return NULL;
    slash = strrchr(path, '/');
    dot = strrchr(slash ? slash : path, '.');
    if (!dot)
        return NULL;
    if (strcmp(dot, ".c") == 0)
        return &langs[LC_LANG_C];
    if (strcmp(dot, ".cpp") == 0 || strcmp(dot, ".cc") == 0 ||
        strcmp(dot, ".cxx") == 0)
        return &langs[LC_LANG_CPP];
    return NULL;
}

/* ------------------------------------------------------------------ */
/* Compilers                                                           */
/* ------------------------------------------------------------------ */

static int is_blank(int c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

int lc_compiler_get(lc_ctx *ctx, const lc_lang *lang, lc_arena *a,
                    lc_compiler *out)
{
    const char *conf = lc_config_get(ctx, config_keys[lang->id].compiler);
    const char *flags = lc_config_get(ctx, config_keys[lang->id].flags);
    const char *cmd = conf ? conf : lc_getenv(lang->compiler_env), *p, *e;
    char *word, *path, *id;
    struct stat st;

    memset(out, 0, sizeof *out);
    if (!cmd)
        cmd = lang->compiler;
    for (p = cmd; is_blank((unsigned char)*p); p++)
        ;
    for (e = p; *e && !is_blank((unsigned char)*e); e++)
        ;
    word = lc_arena_strndup(a, p, (size_t)(e - p));
    if (!word)
        return lc_oom(ctx);
    path = *word ? lc_which(a, word) : NULL;
    if (!path || stat(path, &st) != 0) {
        if (conf)
            return lc_set_error(ctx, LC_ERR_NOTFOUND, "the %s compiler '%s' "
                                "(from %s in %s:%d) was not found",
                                lang->title, cmd,
                                config_keys[lang->id].compiler,
                                lc_config_file(ctx),
                                lc_config_line(ctx,
                                               config_keys[lang->id].compiler));
        if (lc_getenv(lang->compiler_env))
            return lc_set_error(ctx, LC_ERR_NOTFOUND, "the %s compiler '%s' "
                                "(from $%s) was not found", lang->title, cmd,
                                lang->compiler_env);
        return lc_set_error(ctx, LC_ERR_NOTFOUND, "the %s compiler '%s' was "
                            "not found (set %s to use another one)",
                            lang->title, cmd, lang->compiler_env);
    }
    /* What tells this compiler from another, or from another version of
     * itself, in a one-line form for the caches. */
    id = lc_arena_printf(a, "%s|%s|%lld|%lld", cmd, path,
                         (long long)st.st_size, (long long)st.st_mtime);
    out->cmd = lc_arena_strdup(a, cmd);
    out->flags = flags ? lc_arena_strdup(a, flags) : NULL;
    if (!id || !out->cmd || (flags && !out->flags))
        return lc_oom(ctx);
    for (p = id; *p; p++)
        if (*p == '\t' || *p == '\n' || *p == '\r')
            id[p - id] = ' ';
    out->path = path;
    out->id = id;
    return LC_OK;
}

int lc_lang_installed(lc_ctx *ctx, const lc_lang *lang)
{
    lc_arena a = {NULL};
    lc_compiler cc;
    lc_str keep = LC_STR_INIT;
    int ok;

    if (!lang)
        return 0;
    /* Don't let the lookup's message replace the last error. */
    lc_str_puts(&keep, lc_last_error(ctx));
    ok = lc_compiler_get(ctx, lang, &a, &cc) == LC_OK;
    if (!keep.failed)
        lc_set_error(ctx, LC_OK, "%s", lc_str_cstr(&keep));
    lc_str_free(&keep);
    lc_arena_free(&a);
    return ok;
}

/* argv for running the compiler with `args`:
 * /bin/sh -c 'exec CMD "$@" EXTRA'. */
char **lc_compiler_argv(lc_arena *a, const lc_compiler *cc,
                        const char *const *args, size_t nargs,
                        const char *extra)
{
    char **argv = lc_arena_array(a, nargs + 5, sizeof *argv);
    size_t i;

    if (!argv)
        return NULL;
    argv[0] = "/bin/sh";
    argv[1] = "-c";
    argv[2] = lc_arena_printf(a, "exec %s \"$@\"%s%s", cc->cmd,
                              extra && *extra ? " " : "",
                              extra ? extra : "");
    argv[3] = "lc-compile";
    if (!argv[2])
        return NULL;
    for (i = 0; i < nargs; i++)
        argv[4 + i] = (char *)args[i];
    argv[4 + nargs] = NULL;
    return argv;
}

/* ------------------------------------------------------------------ */
/* Standards                                                           */
/* ------------------------------------------------------------------ */

static const char *find_std(const lc_lang *lang, const char *name)
{
    size_t i;

    for (i = 0; lang->stds[i]; i++)
        if (strcmp(lang->stds[i], name) == 0)
            return lang->stds[i];
    return NULL;
}

static const char *alt_of(const char *std)
{
    size_t i;

    for (i = 0; i < sizeof alt_stds / sizeof alt_stds[0]; i++)
        if (strcmp(alt_stds[i].std, std) == 0)
            return alt_stds[i].alt;
    return NULL;
}

/* "c++11, c++14, c++17, c++20, c++23 or max". */
static void list_stds(lc_str *s, const lc_lang *lang)
{
    size_t i;

    for (i = 0; lang->stds[i]; i++)
        lc_str_printf(s, "%s%s", i ? ", " : "", lang->stds[i]);
    lc_str_puts(s, " or max");
}

/* Looks `std` up in the probe cache: 1 accepted (*flag set), 0 not
 * accepted, -1 not known yet. */
static int cached_answer(const lc_str *cache, const char *id,
                         const char *std, const char **flag)
{
    const char *p = lc_str_cstr(cache), *end = p + cache->len;
    size_t idlen = strlen(id), stdlen = strlen(std);

    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *le = nl ? nl : end;

        if ((size_t)(le - p) > idlen + stdlen + 2 &&
            memcmp(p, id, idlen) == 0 && p[idlen] == '\t' &&
            memcmp(p + idlen + 1, std, stdlen) == 0 &&
            p[idlen + 1 + stdlen] == '\t') {
            const char *f = p + idlen + stdlen + 2;
            size_t flen = (size_t)(le - f);
            const char *alt = alt_of(std);

            if (flen == 1 && *f == '-')
                return 0;
            if (flen == stdlen && memcmp(f, std, stdlen) == 0) {
                *flag = std;
                return 1;
            }
            if (alt && flen == strlen(alt) && memcmp(f, alt, flen) == 0) {
                *flag = alt;
                return 1;
            }
        }
        p = le + (nl ? 1 : 0);
    }
    return -1;
}

/* Compiles an empty file with -std=<flag>: 1 if that works, else 0. */
static int probe(lc_ctx *ctx, const lc_lang *lang, const lc_compiler *cc,
                 const char *dir, const char *flag, int *ok)
{
    lc_arena a = {NULL};
    lc_run_spec spec;
    lc_run_result r;
    const char *args[4];
    char **argv;
    char *file;
    int rc;

    *ok = 0;
    file = lc_arena_printf(&a, "%s/probe.%s", dir, lang->ext);
    args[0] = file ? lc_arena_printf(&a, "-std=%s", flag) : NULL;
    args[1] = "-fsyntax-only";
    args[2] = file;
    argv = args[0] ? lc_compiler_argv(&a, cc, args, 3, NULL) : NULL;
    if (!argv) {
        lc_arena_free(&a);
        return lc_oom(ctx);
    }
    rc = lc_write_file_atomic(ctx, file, "typedef int lc_probe;\n", 22);
    if (rc != LC_OK) {
        lc_arena_free(&a);
        return rc;
    }
    memset(&spec, 0, sizeof spec);
    spec.path = argv[0];
    spec.argv = argv;
    spec.cwd = dir;
    spec.wall_ms = PROBE_MS;
    lc_run_result_init(&r);
    rc = lc_run(ctx, &spec, &r);
    if (rc == LC_OK)
        *ok = r.exited && r.exit_code == 0;
    lc_run_result_free(&r);
    lc_arena_free(&a);
    return rc;
}

/*
 * Whether the compiler accepts `std`, and with which -std= spelling:
 * from the probe cache, else by probing (and then remembering the answer).
 */
static int std_supported(lc_ctx *ctx, const lc_lang *lang,
                         const lc_compiler *cc, const char *std,
                         const char **flag, int *ok)
{
    const char *dir = lc_cache_dir(ctx), *alt = alt_of(std);
    lc_str cache = LC_STR_INIT, path = LC_STR_INIT;
    int rc, ans;

    *ok = 0;
    *flag = NULL;
    if (!dir)
        return LC_ERR_NOTFOUND;
    rc = lc_mkdir_p(ctx, dir, 0777);
    if (rc != LC_OK)
        return rc;
    if (lc_str_printf(&path, "%s/stds", dir) != LC_OK)
        return lc_oom(ctx);
    rc = lc_read_file(ctx, path.data, LC_MIB, &cache);
    if (rc == LC_ERR_NOMEM)
        goto done;
    if (rc != LC_OK)
        lc_str_clear(&cache); /* missing or unreadable: start afresh */
    ans = cached_answer(&cache, cc->id, std, flag);
    if (ans >= 0) {
        *ok = ans;
        rc = LC_OK;
        goto done;
    }

    rc = probe(ctx, lang, cc, dir, std, ok);
    if (rc == LC_OK && *ok)
        *flag = std;
    if (rc == LC_OK && !*ok && alt) {
        rc = probe(ctx, lang, cc, dir, alt, ok);
        if (rc == LC_OK && *ok)
            *flag = alt;
    }
    if (rc != LC_OK)
        goto done;
    /* Remember it. A cache that can't be written only costs a probe the
     * next time. */
    if (cache.len > 0 && cache.data[cache.len - 1] != '\n')
        lc_str_putc(&cache, '\n');
    lc_str_printf(&cache, "%s\t%s\t%s\n", cc->id, std, *ok ? *flag : "-");
    if (cache.failed) {
        rc = lc_oom(ctx);
        goto done;
    }
    if (lc_write_file_atomic(ctx, path.data, cache.data, cache.len) ==
        LC_ERR_NOMEM)
        rc = lc_oom(ctx);
done:
    lc_str_free(&cache);
    lc_str_free(&path);
    return rc;
}

int lc_std_pick(lc_ctx *ctx, const lc_lang *lang, const lc_compiler *cc,
                const char *want, const char **name, const char **flag)
{
    const char *from = "--std", *std;
    lc_str list = LC_STR_INIT;
    int rc, ok, i;

    *name = *flag = NULL;
    if (!want) {
        want = lc_getenv(lang->std_env);
        from = lang->std_env;
    }
    if (!want) {
        /* The config file's value was checked when it was read. */
        want = lc_config_get(ctx, config_keys[lang->id].std);
        from = config_keys[lang->id].std;
    }
    if (!want) {
        *name = *flag = lang->default_std;
        return LC_OK;
    }

    if (strcmp(want, "max") == 0) {
        for (i = 0; lang->stds[i]; i++)
            ;
        while (--i >= 0) {
            rc = std_supported(ctx, lang, cc, lang->stds[i], flag, &ok);
            if (rc != LC_OK)
                return rc;
            if (ok) {
                *name = lang->stds[i];
                return LC_OK;
            }
            if (strcmp(lang->stds[i], lang->default_std) == 0)
                break;
        }
        /* Not even the default works; let the build say why. */
        *name = *flag = lang->default_std;
        return LC_OK;
    }

    std = find_std(lang, want);
    if (!std) {
        list_stds(&list, lang);
        if (list.failed) {
            lc_str_free(&list);
            return lc_oom(ctx);
        }
        if (from[0] == '-')
            rc = lc_set_error(ctx, LC_ERR_INVALID, "unknown %s standard '%s' "
                              "(expected %s)", lang->title, want, list.data);
        else if (from == config_keys[lang->id].std)
            rc = lc_set_error(ctx, LC_ERR_INVALID, "unknown %s standard '%s' "
                              "in %s:%d (expected %s)", lang->title, want,
                              lc_config_file(ctx), lc_config_line(ctx, from),
                              list.data);
        else
            rc = lc_set_error(ctx, LC_ERR_INVALID, "unknown %s standard '%s' "
                              "in $%s (expected %s)", lang->title, want, from,
                              list.data);
        lc_str_free(&list);
        return rc;
    }
    if (strcmp(std, lang->default_std) == 0) {
        *name = *flag = std;
        return LC_OK;
    }
    rc = std_supported(ctx, lang, cc, std, flag, &ok);
    if (rc != LC_OK)
        return rc;
    if (!ok)
        return lc_set_error(ctx, LC_ERR_INVALID, "the %s compiler '%s' does "
                            "not support %s (try an older standard, or "
                            "'max' for the newest it has)", lang->title,
                            cc->cmd, std);
    *name = std;
    return LC_OK;
}

int lc_std_resolve(lc_ctx *ctx, const lc_lang *lang, const char *want,
                   const char **out)
{
    lc_arena a = {NULL};
    lc_compiler cc;
    const char *flag;
    int rc;

    if (!lang || !out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_std_resolve: bad "
                            "arguments");
    *out = NULL;
    rc = lc_compiler_get(ctx, lang, &a, &cc);
    if (rc == LC_OK)
        rc = lc_std_pick(ctx, lang, &cc, want, out, &flag);
    lc_arena_free(&a);
    return rc;
}
