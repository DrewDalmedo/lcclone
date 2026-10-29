#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "internal.h"
#include "test.h"

#define TMP "tests/tmp/fs"

static void test_mkdir_and_write(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_str s = LC_STR_INIT;
    lc_arena a = {0};
    char **names;
    size_t n;

    CHECK_INT(lc_mkdir_p(ctx, TMP "/a/b//c/", 0777), LC_OK);
    CHECK(lc_is_dir(TMP "/a/b/c"));
    CHECK_INT(lc_mkdir_p(ctx, TMP "/a/b/c", 0777), LC_OK); /* exists: fine */

    CHECK_INT(lc_write_file_atomic(ctx, TMP "/a/f.txt", "hello\n", 6), LC_OK);
    CHECK(lc_is_file(TMP "/a/f.txt"));
    CHECK(!lc_is_dir(TMP "/a/f.txt"));
    CHECK_INT(lc_read_file(ctx, TMP "/a/f.txt", &s), LC_OK);
    CHECK_STR(s.data, "hello\n");

    /* Overwrite atomically, then append on read. */
    CHECK_INT(lc_write_file_atomic(ctx, TMP "/a/f.txt", "bye", 3), LC_OK);
    CHECK_INT(lc_read_file(ctx, TMP "/a/f.txt", &s), LC_OK);
    CHECK_STR(s.data, "hello\nbye");

    /* An empty file reads as empty. */
    CHECK_INT(lc_write_file_atomic(ctx, TMP "/a/empty", "", 0), LC_OK);
    lc_str_clear(&s);
    CHECK_INT(lc_read_file(ctx, TMP "/a/empty", &s), LC_OK);
    CHECK_INT(s.len, 0);

    /* mkdir -p through a file fails with a message. */
    CHECK_INT(lc_mkdir_p(ctx, TMP "/a/f.txt/x", 0777), LC_ERR_IO);
    CHECK(strstr(lc_last_error(ctx), "f.txt") != NULL);

    /* No temp files are left behind. */
    CHECK_INT(lc_list_dir(ctx, TMP "/a", &a, &names, &n), LC_OK);
    CHECK_INT(n, 3);
    if (n == 3) {
        CHECK_STR(names[0], "b");
        CHECK_STR(names[1], "empty");
        CHECK_STR(names[2], "f.txt");
    }
    lc_arena_free(&a);
    lc_str_free(&s);
    lc_close(ctx);
}

static void test_read_errors(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_str s = LC_STR_INIT;
    lc_arena a = {0};
    char **names;
    size_t n;

    CHECK_INT(lc_read_file(ctx, TMP "/missing", &s), LC_ERR_NOTFOUND);
    CHECK(strstr(lc_last_error(ctx), "missing") != NULL);
    CHECK_INT(lc_read_file(ctx, TMP, &s), LC_ERR_IO);
    CHECK_INT(lc_list_dir(ctx, TMP "/missing", &a, &names, &n), LC_ERR_NOTFOUND);
    CHECK(names == NULL && n == 0);
    lc_arena_free(&a);
    lc_str_free(&s);
    lc_close(ctx);
}

static void test_paths(void)
{
    lc_arena a = {0};
    char *r;

    CHECK_STR(lc_path_join(&a, "a", "b"), "a/b");
    CHECK_STR(lc_path_join(&a, "a/", "b"), "a/b");
    CHECK_STR(lc_path_join(&a, "a///", "b"), "a/b");
    CHECK_STR(lc_path_join(&a, "/", "b"), "/b");
    CHECK_STR(lc_path_join(&a, "", "b"), "b");
    CHECK_STR(lc_path_join(&a, "a", "/abs"), "/abs");
    r = lc_realpath(&a, ".");
    CHECK(r != NULL && r[0] == '/');
    CHECK(lc_realpath(&a, TMP "/does/not/exist") == NULL);
    lc_arena_free(&a);
}

static void test_env(void)
{
    lc_arena a = {0};

    setenv("LC_TEST_EMPTY", "", 1);
    CHECK(lc_getenv("LC_TEST_EMPTY") == NULL);
    setenv("LC_TEST_SET", "v", 1);
    CHECK_STR(lc_getenv("LC_TEST_SET"), "v");

    setenv("HOME", "/home/someone", 1);
    setenv("XDG_DATA_HOME", "/xdg/data", 1);
    CHECK_STR(lc_xdg_dir(&a, "XDG_DATA_HOME", ".local/share"), "/xdg/data");
    setenv("XDG_DATA_HOME", "relative/ignored", 1);
    CHECK_STR(lc_xdg_dir(&a, "XDG_DATA_HOME", ".local/share"),
              "/home/someone/.local/share");
    unsetenv("XDG_DATA_HOME");
    CHECK_STR(lc_xdg_dir(&a, "XDG_DATA_HOME", ".local/share"),
              "/home/someone/.local/share");
    unsetenv("HOME");
    CHECK(lc_xdg_dir(&a, "XDG_DATA_HOME", ".local/share") == NULL);
    lc_arena_free(&a);
}

int main(void)
{
    lc_ctx *ctx = lc_open(NULL);

    /* Start from a clean scratch directory. */
    if (system("rm -rf " TMP) != 0)
        return 1;
    lc_mkdir_p(ctx, TMP, 0777);
    lc_close(ctx);
    RUN(test_mkdir_and_write);
    RUN(test_read_errors);
    RUN(test_paths);
    RUN(test_env);
    return test_report();
}
