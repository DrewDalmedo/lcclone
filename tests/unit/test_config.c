#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* The config file (config.c), and how its settings reach the core. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "internal.h"
#include "test.h"

#define TMP "tests/tmp/config"

static void write_bytes(const char *path, const char *data, size_t n)
{
    FILE *f = fopen(path, "wb");

    CHECK(f != NULL);
    if (!f)
        return;
    CHECK(fwrite(data, 1, n, f) == n);
    CHECK(fclose(f) == 0);
}

static void write_file(const char *path, const char *data)
{
    write_bytes(path, data, strlen(data));
}

static void make_dir(const char *path)
{
    lc_ctx *ctx = lc_open(NULL);

    CHECK_INT(lc_mkdir_p(ctx, path, 0777), LC_OK);
    lc_close(ctx);
}

/* Opens a context that reads `text` as its config file. */
static lc_ctx *open_with(const char *text)
{
    lc_options opt;

    write_file(TMP "/config", text);
    lc_options_init(&opt);
    opt.config_file = TMP "/config";
    return lc_open(&opt);
}

static void test_values(void)
{
    lc_ctx *ctx;

    setenv("HOME", "/home/me", 1);
    ctx = open_with("# comment\n"
                    "\n"
                    "cxxstd = c++17\n"
                    "  editor   =   nvim -O  \n"
                    "cflags=-O0 -g\n"
                    "workspace = ~/solutions\n"
                    "lang = c\n"
                    "color = never\n"
                    "open_statement = yes\n");
    CHECK(ctx != NULL);
    CHECK_INT(lc_warning_count(ctx), 0);
    CHECK_STR(lc_last_error(ctx), "");
    CHECK_STR(lc_config_file(ctx), TMP "/config");
    CHECK_STR(lc_config_get(ctx, "cxxstd"), "c++17");
    CHECK_STR(lc_config_get(ctx, "editor"), "nvim -O");
    CHECK_STR(lc_config_get(ctx, "cflags"), "-O0 -g");
    CHECK_STR(lc_config_get(ctx, "workspace"), "/home/me/solutions");
    CHECK_STR(lc_config_get(ctx, "lang"), "c");
    CHECK_STR(lc_config_get(ctx, "color"), "never");
    CHECK_STR(lc_config_get(ctx, "open_statement"), "yes");
    CHECK(lc_config_get(ctx, "cc") == NULL);
    CHECK(lc_config_get(ctx, "nonsense") == NULL);
    CHECK(lc_config_get(ctx, NULL) == NULL);
    CHECK_INT(lc_config_line(ctx, "editor"), 4);
    CHECK_INT(lc_config_line(ctx, "cc"), 0);
    /* The workspace setting is used when nothing more specific is set. */
    unsetenv("LC_WORKSPACE");
    CHECK_STR(lc_workspace_dir(ctx), "/home/me/solutions");
    lc_close(ctx);

    /* A later line wins; an empty value unsets the key. */
    ctx = open_with("cc = gcc\ncc = clang\ncxx = g++\ncxx =\r\n");
    CHECK_INT(lc_warning_count(ctx), 0);
    CHECK_STR(lc_config_get(ctx, "cc"), "clang");
    CHECK_INT(lc_config_line(ctx, "cc"), 2);
    CHECK(lc_config_get(ctx, "cxx") == NULL);
    lc_close(ctx);

    /* A byte order mark and CRLF line ends. */
    ctx = open_with("\xEF\xBB\xBF" "cstd = c17\r\nworkspace = ~\r\n");
    CHECK_INT(lc_warning_count(ctx), 0);
    CHECK_STR(lc_config_get(ctx, "cstd"), "c17");
    CHECK_STR(lc_config_get(ctx, "workspace"), "/home/me");
    lc_close(ctx);
}

static void test_bad_lines(void)
{
    lc_ctx *ctx = open_with("bogus = 1\n"
                            "no equals here\n"
                            "= value\n"
                            "Editor = vi\n"
                            "color = sometimes\n"
                            "lang = rust\n"
                            "cxxstd = c++99\n"
                            "cstd = c++17\n"
                            "open_statement = maybe\n"
                            "workspace = relative/dir\n"
                            "cc = gcc\n");

    CHECK(ctx != NULL);
    CHECK_INT(lc_warning_count(ctx), 10);
    CHECK_STR(lc_warning_at(ctx, 0), TMP "/config:1: unknown key 'bogus'");
    CHECK_STR(lc_warning_at(ctx, 1), TMP "/config:2: expected 'key = value', "
              "got 'no equals here'");
    CHECK_STR(lc_warning_at(ctx, 2), TMP "/config:3: expected 'key = value', "
              "got '= value'");
    CHECK_STR(lc_warning_at(ctx, 3), TMP "/config:4: expected 'key = value', "
              "got 'Editor = vi'");
    CHECK_STR(lc_warning_at(ctx, 4), TMP "/config:5: color: invalid value "
              "'sometimes' (expected auto, always or never)");
    CHECK_STR(lc_warning_at(ctx, 5), TMP "/config:6: lang: invalid value "
              "'rust' (expected c or cpp)");
    CHECK_STR(lc_warning_at(ctx, 6), TMP "/config:7: cxxstd: invalid value "
              "'c++99' (expected c++11, c++14, c++17, c++20, c++23 or max)");
    CHECK_STR(lc_warning_at(ctx, 7), TMP "/config:8: cstd: invalid value "
              "'c++17' (expected c99, c11, c17, c23 or max)");
    CHECK_STR(lc_warning_at(ctx, 8), TMP "/config:9: open_statement: invalid "
              "value 'maybe' (expected yes or no)");
    CHECK_STR(lc_warning_at(ctx, 9), TMP "/config:10: workspace: invalid "
              "value 'relative/dir' (expected an absolute path, or one "
              "starting with '~/')");
    /* The good line still counts; the bad ones are left unset. */
    CHECK_STR(lc_config_get(ctx, "cc"), "gcc");
    CHECK(lc_config_get(ctx, "color") == NULL);
    CHECK(lc_config_get(ctx, "cxxstd") == NULL);
    lc_close(ctx);

    {
        static const char text[] = "cc = g\0cc\ncxx = g++\n";

        lc_options opt;

        write_bytes(TMP "/config", text, sizeof text - 1);
        lc_options_init(&opt);
        opt.config_file = TMP "/config";
        ctx = lc_open(&opt);
        CHECK_INT(lc_warning_count(ctx), 1);
        CHECK_STR(lc_warning_at(ctx, 0), TMP "/config:1: line contains a NUL "
                  "byte");
        CHECK(lc_config_get(ctx, "cc") == NULL);
        CHECK_STR(lc_config_get(ctx, "cxx"), "g++");
        lc_close(ctx);
    }
}

static void test_where(void)
{
    lc_options opt;
    lc_ctx *ctx;

    /* config_file "" means no config at all. */
    lc_options_init(&opt);
    opt.config_file = "";
    ctx = lc_open(&opt);
    CHECK(lc_config_file(ctx) == NULL);
    CHECK_INT(lc_warning_count(ctx), 0);
    lc_close(ctx);

    /* A file named explicitly must exist. */
    opt.config_file = TMP "/missing";
    ctx = lc_open(&opt);
    CHECK_INT(lc_warning_count(ctx), 1);
    {
        const char *want = "cannot read the config file '" TMP "/missing': ";

        CHECK(strncmp(lc_warning_at(ctx, 0), want, strlen(want)) == 0);
    }
    lc_close(ctx);

    /* $LC_CONFIG, named in the warning. */
    setenv("LC_CONFIG", TMP "/missing", 1);
    ctx = lc_open(NULL);
    CHECK_INT(lc_warning_count(ctx), 1);
    CHECK(strstr(lc_warning_at(ctx, 0), "(from $LC_CONFIG)") != NULL);
    lc_close(ctx);
    write_file(TMP "/env-config", "lang = c\n");
    setenv("LC_CONFIG", TMP "/env-config", 1);
    ctx = lc_open(NULL);
    CHECK_STR(lc_config_get(ctx, "lang"), "c");
    lc_close(ctx);
    unsetenv("LC_CONFIG");

    /* Then $XDG_CONFIG_HOME/lcclone/config; missing there is no config. */
    make_dir(TMP "/xdg/lcclone");
    setenv("XDG_CONFIG_HOME", "/nonexistent-lc-config", 1);
    ctx = lc_open(NULL);
    CHECK_STR(lc_config_file(ctx), "/nonexistent-lc-config/lcclone/config");
    CHECK_INT(lc_warning_count(ctx), 0);
    lc_close(ctx);
    {
        char *cwd = getcwd(NULL, 0), path[4096];

        snprintf(path, sizeof path, "%s/" TMP "/xdg", cwd);
        setenv("XDG_CONFIG_HOME", path, 1);
        write_file(TMP "/xdg/lcclone/config", "cxx = clang++\n");
        ctx = lc_open(NULL);
        CHECK_STR(lc_config_get(ctx, "cxx"), "clang++");
        lc_close(ctx);
        free(cwd);
    }
    /* Then ~/.config/lcclone/config. */
    unsetenv("XDG_CONFIG_HOME");
    setenv("HOME", "/home/someone", 1);
    ctx = lc_open(NULL);
    CHECK_STR(lc_config_file(ctx), "/home/someone/.config/lcclone/config");
    lc_close(ctx);
    unsetenv("HOME");
    ctx = lc_open(NULL);
    CHECK(lc_config_file(ctx) == NULL);
    lc_close(ctx);

    /* A directory is not a config file. */
    opt.config_file = TMP;
    ctx = lc_open(&opt);
    CHECK_INT(lc_warning_count(ctx), 1);
    CHECK_STR(lc_warning_at(ctx, 0), "cannot read the config file '" TMP
              "': not a regular file");
    lc_close(ctx);
}

static void test_bool(void)
{
    CHECK_INT(lc_config_bool("yes"), 1);
    CHECK_INT(lc_config_bool("true"), 1);
    CHECK_INT(lc_config_bool("on"), 1);
    CHECK_INT(lc_config_bool("1"), 1);
    CHECK_INT(lc_config_bool("no"), 0);
    CHECK_INT(lc_config_bool("false"), 0);
    CHECK_INT(lc_config_bool("off"), 0);
    CHECK_INT(lc_config_bool("0"), 0);
    CHECK_INT(lc_config_bool("Yes"), -1);
    CHECK_INT(lc_config_bool(""), -1);
    CHECK_INT(lc_config_bool(NULL), -1);
}

/* The config's standard comes after --std and $LC_CXXSTD. */
static void test_std_order(void)
{
    const lc_lang *cpp = lc_lang_find("cpp");
    lc_ctx *ctx;
    const char *std;
    lc_options opt;
    char *cwd = getcwd(NULL, 0), cache[4096];

    snprintf(cache, sizeof cache, "%s/" TMP "/cache", cwd);
    free(cwd);
    write_file(TMP "/config", "cxxstd = c++11\n");
    lc_options_init(&opt);
    opt.config_file = TMP "/config";
    opt.cache_dir = cache;
    unsetenv("LC_CXXSTD");
    ctx = lc_open(&opt);
    if (!lc_lang_installed(ctx, cpp)) {
        printf("test_config: no C++ compiler; skipped the standard checks\n");
        lc_close(ctx);
        return;
    }
    CHECK_INT(lc_std_resolve(ctx, cpp, NULL, &std), LC_OK);
    CHECK_STR(std, "c++11");
    setenv("LC_CXXSTD", "c++14", 1);
    CHECK_INT(lc_std_resolve(ctx, cpp, NULL, &std), LC_OK);
    CHECK_STR(std, "c++14");
    CHECK_INT(lc_std_resolve(ctx, cpp, "c++11", &std), LC_OK);
    CHECK_STR(std, "c++11");
    unsetenv("LC_CXXSTD");
    lc_close(ctx);
}

int main(void)
{
    make_dir(TMP);
    RUN(test_values);
    RUN(test_bad_lines);
    RUN(test_where);
    RUN(test_bool);
    RUN(test_std_order);
    return test_report();
}
