#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * config.c: the config file (docs/workspace.md, "Configuration").
 *
 *     # ~/.config/lcclone/config
 *     cxxstd = c++17
 *     editor = nvim -O
 *     open_statement = yes
 *
 * One `key = value` per line; blank lines and lines starting with '#'
 * are ignored. lc_open reads it once. A line that can't be used is
 * skipped with a warning naming the file and line, so a mistake in the
 * config never stops lc from working.
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "internal.h"

/* Largest config file read. */
#define MAX_CONFIG LC_MIB

/* What a key's value must look like. */
enum { V_TEXT, V_PATH, V_LANG, V_CSTD, V_CXXSTD, V_COLOR, V_BOOL };

static const struct {
    const char *key;
    int kind;
} keys[] = {
    {"cc", V_TEXT},          {"cxx", V_TEXT},     {"cstd", V_CSTD},
    {"cxxstd", V_CXXSTD},    {"cflags", V_TEXT},  {"cxxflags", V_TEXT},
    {"editor", V_TEXT},      {"lang", V_LANG},    {"color", V_COLOR},
    {"workspace", V_PATH},   {"open_statement", V_BOOL},
};

#define NKEYS (sizeof keys / sizeof keys[0])

static int is_blank(int c)
{
    return c == ' ' || c == '\t';
}

static int key_char(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
}

static int find_key(const char *p, size_t n)
{
    size_t i;

    for (i = 0; i < NKEYS; i++)
        if (strlen(keys[i].key) == n && memcmp(keys[i].key, p, n) == 0)
            return (int)i;
    return -1;
}

static int in_list(const char *const *list, const char *v)
{
    for (; *list; list++)
        if (strcmp(*list, v) == 0)
            return 1;
    return 0;
}

int lc_config_bool(const char *v)
{
    static const char *const yes[] = {"yes", "true", "on", "1", NULL};
    static const char *const no[] = {"no", "false", "off", "0", NULL};

    if (!v)
        return -1;
    if (in_list(yes, v))
        return 1;
    if (in_list(no, v))
        return 0;
    return -1;
}

/*
 * Checks a value against its key's kind. Returns NULL when it is fine,
 * else what is wrong with it. *value may be replaced (a "~/" path).
 */
static const char *check_value(lc_ctx *ctx, int kind, char **value)
{
    static const char *const colors[] = {"auto", "always", "never", NULL};
    const lc_lang *lang;
    char *v = *value;

    switch (kind) {
    case V_LANG:
        return lc_lang_find(v) ? NULL : "expected c or cpp";
    case V_COLOR:
        return in_list(colors, v) ? NULL : "expected auto, always or never";
    case V_BOOL:
        return lc_config_bool(v) >= 0 ? NULL : "expected yes or no";
    case V_CSTD:
    case V_CXXSTD:
        lang = lc_lang_at(kind == V_CSTD ? LC_LANG_C : LC_LANG_CPP);
        if (strcmp(v, "max") == 0 || in_list(lang->stds, v))
            return NULL;
        return kind == V_CSTD ? "expected c99, c11, c17, c23 or max"
                              : "expected c++11, c++14, c++17, c++20, c++23 "
                                "or max";
    case V_PATH:
        if (v[0] == '/')
            return NULL;
        if (v[0] == '~' && (v[1] == '\0' || v[1] == '/')) {
            const char *home = lc_getenv("HOME");
            char *full;

            if (!home)
                return "'~' needs HOME to be set";
            full = lc_arena_printf(&ctx->arena, "%s%s", home, v + 1);
            *value = full; /* NULL when out of memory: the caller checks */
            return NULL;
        }
        return "expected an absolute path, or one starting with '~/'";
    default:
        return NULL;
    }
}

static int config_warn(lc_ctx *ctx, const char *path, int line,
                       const char *fmt, ...) LC_PRINTF(4, 5);

static int config_warn(lc_ctx *ctx, const char *path, int line,
                       const char *fmt, ...)
{
    lc_str msg = LC_STR_INIT;
    va_list ap;
    int rc;

    va_start(ap, fmt);
    lc_str_vprintf(&msg, fmt, ap);
    va_end(ap);
    if (msg.failed) {
        lc_str_free(&msg);
        return lc_oom(ctx);
    }
    rc = lc_add_warning(ctx, "%s:%d: %s", path, line, msg.data);
    lc_str_free(&msg);
    return rc;
}

/* Parses the text of a config file into ctx->config. */
static int parse_config(lc_ctx *ctx, const char *path, const char *text,
                        size_t len)
{
    const char *p = text, *end = text + len;
    int lineno = 0, rc;

    if (len >= 3 && memcmp(p, "\xEF\xBB\xBF", 3) == 0)
        p += 3;
    while (p < end) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        const char *ls = p, *le = nl ? nl : end, *k, *ke, *v, *ve;
        const char *problem;
        lc_config_entry *e;
        char *value;
        size_t i;
        int idx;

        p = nl ? nl + 1 : end;
        lineno++;
        if (le > ls && le[-1] == '\r')
            le--;
        while (ls < le && is_blank((unsigned char)*ls))
            ls++;
        while (le > ls && is_blank((unsigned char)le[-1]))
            le--;
        if (ls == le || *ls == '#')
            continue;
        if (memchr(ls, '\0', (size_t)(le - ls))) {
            rc = config_warn(ctx, path, lineno, "line contains a NUL byte");
            if (rc != LC_OK)
                return rc;
            continue;
        }
        for (k = ke = ls; ke < le && key_char((unsigned char)*ke); ke++)
            ;
        for (v = ke; v < le && is_blank((unsigned char)*v); v++)
            ;
        if (ke == k || v == le || *v != '=') {
            rc = config_warn(ctx, path, lineno, "expected 'key = value', "
                             "got '%.*s'", (int)(le - ls > 60 ? 60 : le - ls),
                             ls);
            if (rc != LC_OK)
                return rc;
            continue;
        }
        for (v++; v < le && is_blank((unsigned char)*v); v++)
            ;
        ve = le;
        idx = find_key(k, (size_t)(ke - k));
        if (idx < 0) {
            rc = config_warn(ctx, path, lineno, "unknown key '%.*s'",
                             (int)(ke - k > 60 ? 60 : ke - k), k);
            if (rc != LC_OK)
                return rc;
            continue;
        }
        /* An empty value unsets the key, as an empty variable does. */
        for (i = 0; i < ctx->nconfig; i++)
            if (strcmp(ctx->config[i].key, keys[idx].key) == 0)
                break;
        if (v == ve) {
            if (i < ctx->nconfig)
                ctx->config[i] = ctx->config[--ctx->nconfig];
            continue;
        }
        value = lc_arena_strndup(&ctx->arena, v, (size_t)(ve - v));
        if (!value)
            return lc_oom(ctx);
        problem = check_value(ctx, keys[idx].kind, &value);
        if (!value)
            return lc_oom(ctx);
        if (problem) {
            rc = config_warn(ctx, path, lineno, "%s: invalid value '%s' (%s)",
                             keys[idx].key, value, problem);
            if (rc != LC_OK)
                return rc;
            continue;
        }
        /* A key given twice: the later line wins. */
        if (i == ctx->nconfig) {
            lc_config_entry *tmp = lc_grow(ctx->config, &ctx->capconfig,
                                           ctx->nconfig + 1,
                                           sizeof *ctx->config);
            if (!tmp)
                return lc_oom(ctx);
            ctx->config = tmp;
            ctx->nconfig++;
        }
        e = &ctx->config[i];
        e->key = keys[idx].key;
        e->value = value;
        e->line = lineno;
    }
    return LC_OK;
}

int lc_config_read(lc_ctx *ctx)
{
    const char *given = ctx->opt_config_file, *env = NULL;
    lc_str text = LC_STR_INIT;
    struct stat st;
    char *path, *base;
    int rc;

    if (given && !*given)
        return LC_OK; /* no config at all */
    if (!given)
        env = given = lc_getenv("LC_CONFIG");
    if (given) {
        path = lc_arena_strdup(&ctx->arena, given);
    } else {
        base = lc_xdg_dir(&ctx->arena, "XDG_CONFIG_HOME", ".config");
        if (!base)
            return lc_getenv("HOME") ? lc_oom(ctx) : LC_OK;
        path = lc_path_join(&ctx->arena, base, "lcclone/config");
    }
    if (!path)
        return lc_oom(ctx);
    ctx->config_file = path;

    /* Only a regular file: a FIFO could block forever. */
    if (stat(path, &st) != 0) {
        if (!given && (errno == ENOENT || errno == ENOTDIR))
            return LC_OK;
        return lc_add_warning(ctx, "cannot read the config file '%s'%s: %s",
                              path, env ? " (from $LC_CONFIG)" : "",
                              strerror(errno));
    }
    if (!S_ISREG(st.st_mode))
        return lc_add_warning(ctx, "cannot read the config file '%s': not a "
                              "regular file", path);
    rc = lc_read_file(ctx, path, MAX_CONFIG, &text);
    if (rc == LC_ERR_NOMEM || ctx->err_nomem)
        rc = lc_oom(ctx);
    else if (rc != LC_OK)
        rc = lc_add_warning(ctx, "%s", lc_last_error(ctx));
    else
        rc = parse_config(ctx, path, lc_str_cstr(&text), text.len);
    lc_str_free(&text);
    return rc;
}

const char *lc_config_file(const lc_ctx *ctx)
{
    return ctx->config_file;
}

static const lc_config_entry *find_entry(const lc_ctx *ctx, const char *key)
{
    size_t i;

    if (!key)
        return NULL;
    for (i = 0; i < ctx->nconfig; i++)
        if (strcmp(ctx->config[i].key, key) == 0)
            return &ctx->config[i];
    return NULL;
}

const char *lc_config_get(const lc_ctx *ctx, const char *key)
{
    const lc_config_entry *e = find_entry(ctx, key);

    return e ? e->value : NULL;
}

int lc_config_line(const lc_ctx *ctx, const char *key)
{
    const lc_config_entry *e = find_entry(ctx, key);

    return e ? e->line : 0;
}
