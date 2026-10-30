/* fs.c is compiled into this file (below), and it needs realpath(). */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "internal.h"
#include "test.h"

/*
 * Allocation failures. arena.c, str.c, ctx.c and fs.c are compiled into
 * this file with malloc, calloc, realloc and free routed through the
 * wrappers below, which can make one chosen allocation fail. (Linking
 * liblc.a as well is harmless: it then contributes nothing.) Every header
 * those files include is included above, before the macros are defined.
 */
static struct {
    int armed;    /* count, and maybe fail, the allocations made now */
    long count;   /* allocations attempted while armed */
    long fail_at; /* the one that fails (counting from 0), or -1 */
    long live;    /* blocks allocated and not yet freed while armed */
} oom = {0, 0, -1, 0};

static int oom_fail(void)
{
    if (!oom.armed)
        return 0;
    return oom.count++ == oom.fail_at;
}

static void *oom_malloc(size_t n)
{
    void *p = oom_fail() ? NULL : malloc(n);

    if (p && oom.armed)
        oom.live++;
    return p;
}

static void *oom_calloc(size_t count, size_t size)
{
    void *p = oom_fail() ? NULL : calloc(count, size);

    if (p && oom.armed)
        oom.live++;
    return p;
}

static void *oom_realloc(void *p, size_t n)
{
    void *q = oom_fail() ? NULL : realloc(p, n);

    if (q && !p && oom.armed)
        oom.live++;
    return q;
}

static void oom_free(void *p)
{
    if (p && oom.armed)
        oom.live--;
    free(p);
}

#define malloc(n) oom_malloc(n)
#define calloc(count, size) oom_calloc(count, size)
#define realloc(p, n) oom_realloc(p, n)
#define free(p) oom_free(p)
#include "arena.c"
#include "str.c"
#include "ctx.c"
#include "fs.c"
#undef malloc
#undef calloc
#undef realloc
#undef free

#define TMP "tests/tmp/fs"
#define BIG LC_MIB

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
    CHECK_INT(lc_read_file(ctx, TMP "/a/f.txt", BIG, &s), LC_OK);
    CHECK_STR(s.data, "hello\n");

    /* Overwrite atomically, then append on read. */
    CHECK_INT(lc_write_file_atomic(ctx, TMP "/a/f.txt", "bye", 3), LC_OK);
    CHECK_INT(lc_read_file(ctx, TMP "/a/f.txt", BIG, &s), LC_OK);
    CHECK_STR(s.data, "hello\nbye");

    /* An empty file reads as empty. */
    CHECK_INT(lc_write_file_atomic(ctx, TMP "/a/empty", "", 0), LC_OK);
    lc_str_clear(&s);
    CHECK_INT(lc_read_file(ctx, TMP "/a/empty", BIG, &s), LC_OK);
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

    CHECK_INT(lc_read_file(ctx, TMP "/missing", BIG, &s), LC_ERR_NOTFOUND);
    CHECK(strstr(lc_last_error(ctx), "missing") != NULL);
    CHECK_INT(lc_read_file(ctx, TMP, BIG, &s), LC_ERR_IO);
    CHECK_INT(lc_list_dir(ctx, TMP "/missing", &a, &names, &n), LC_ERR_NOTFOUND);
    CHECK(names == NULL && n == 0);
    lc_arena_free(&a);
    lc_str_free(&s);
    lc_close(ctx);
}

/* A sparse file of `size` bytes. */
static void make_sparse(const char *path, off_t size)
{
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    CHECK(fd >= 0);
    if (fd < 0)
        return;
    CHECK(ftruncate(fd, size) == 0);
    close(fd);
}

/* Files over the limit fail with LC_ERR_IO instead of being read whole. */
static void test_read_limit(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_str s = LC_STR_INIT;
    size_t len;

    CHECK_INT(lc_mkdir_p(ctx, TMP "/lim", 0777), LC_OK);
    CHECK_INT(lc_write_file_atomic(ctx, TMP "/lim/ten", "0123456789", 10),
              LC_OK);
    /* Exactly at the limit is fine; the limit counts this file only. */
    CHECK_INT(lc_str_puts(&s, "abc"), LC_OK);
    CHECK_INT(lc_read_file(ctx, TMP "/lim/ten", 10, &s), LC_OK);
    CHECK_STR(s.data, "abc0123456789");
    lc_str_clear(&s);
    CHECK_INT(lc_read_file(ctx, TMP "/lim/ten", 9, &s), LC_ERR_IO);
    CHECK_STR(lc_last_error(ctx), "cannot read '" TMP "/lim/ten': file too "
              "large (over 9 bytes)");
    CHECK_INT(s.len, 0);
    CHECK_INT(lc_write_file_atomic(ctx, TMP "/lim/empty", "", 0), LC_OK);
    CHECK_INT(lc_read_file(ctx, TMP "/lim/empty", 0, &s), LC_OK);
    CHECK_INT(lc_read_file(ctx, TMP "/lim/ten", 0, &s), LC_ERR_IO);

    /* A huge file is refused without reserving memory for it. */
    make_sparse(TMP "/lim/huge", (off_t)3 * (off_t)LC_MIB);
    lc_str_free(&s);
    CHECK_INT(lc_read_file(ctx, TMP "/lim/huge", 2 * LC_MIB, &s), LC_ERR_IO);
    CHECK_STR(lc_last_error(ctx), "cannot read '" TMP "/lim/huge': file too "
              "large (over 2 MiB)");
    CHECK(s.cap < LC_MIB);
    unlink(TMP "/lim/huge");

    /* A file whose size fstat can't tell (an endless device here) stops
     * one byte past the limit. */
    if (access("/dev/zero", R_OK) == 0) {
        lc_str_free(&s);
        CHECK_INT(lc_str_puts(&s, "x"), LC_OK);
        len = s.len;
        CHECK_INT(lc_read_file(ctx, "/dev/zero", 10000, &s), LC_ERR_IO);
        CHECK_STR(lc_last_error(ctx), "cannot read '/dev/zero': file too "
                  "large (over 10000 bytes)");
        CHECK_INT(s.len - len, 10001);
        CHECK(s.cap < 64 * 1024);
    }
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

/*
 * lc_list_dir with each allocation failing in turn: every failure must give
 * LC_ERR_NOMEM with no output, no leak and no double free (ASan and
 * valgrind catch those). The directory has enough entries to make the name
 * array grow, and the arena needs a new block partway through.
 */
static void test_list_dir_oom(void)
{
    char path[256];
    long k;
    int i, saw_ok = 0;

    CHECK_INT(system("mkdir -p " TMP "/many"), 0);
    for (i = 0; i < 40; i++) {
        FILE *f;

        snprintf(path, sizeof path, TMP "/many/entry-with-a-longish-name-%03d-"
                 "padding-padding-padding-padding-padding-padding-padding-"
                 "padding-padding-padding-padding-padding-padding-padding",
                 i);
        f = fopen(path, "w");
        CHECK(f != NULL);
        if (f)
            fclose(f);
    }
    for (k = 0; k < 200 && !saw_ok; k++) {
        lc_ctx *ctx = lc_open(NULL);
        lc_arena a = {0};
        char **names = (char **)1;
        size_t n = 99;
        int rc;

        oom.count = 0;
        oom.fail_at = k;
        oom.live = 0;
        oom.armed = 1;
        rc = lc_list_dir(ctx, TMP "/many", &a, &names, &n);
        lc_arena_free(&a);
        oom.armed = 0;
        if (rc == LC_OK) {
            saw_ok = 1;
            CHECK_INT(n, 40);
        } else {
            CHECK_INT(rc, LC_ERR_NOMEM);
            CHECK(names == NULL && n == 0);
            CHECK_STR(lc_last_error(ctx), "out of memory");
        }
        CHECK_INT(oom.live, 0);
        lc_close(ctx);
    }
    CHECK(saw_ok);
    CHECK(k > 3); /* several allocations were really failed */
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
    RUN(test_read_limit);
    RUN(test_paths);
    RUN(test_env);
    RUN(test_list_dir_oom);
    return test_report();
}
