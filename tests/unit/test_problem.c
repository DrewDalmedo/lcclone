#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "internal.h"
#include "test.h"

#define TMP "tests/tmp/problem"
#define FIXTURES "tests/fixtures/problems"
#define BAD "tests/fixtures/bad"

/* The four required fields; the next header line is line 5. */
#define HDR(id, slug) "id: " id "\nslug: " slug "\ntitle: T\ndifficulty: easy\n"

static const char not_found_hint[] =
    "; set LC_PROBLEMS to the directory that holds the problem packs";

static void make_dir(const char *path)
{
    lc_ctx *ctx = lc_open(NULL);
    CHECK_INT(lc_mkdir_p(ctx, path, 0777), LC_OK);
    lc_close(ctx);
}

static void write_file(const char *path, const char *data)
{
    FILE *f = fopen(path, "wb");

    CHECK(f != NULL);
    if (!f)
        return;
    CHECK(fwrite(data, 1, strlen(data), f) == strlen(data));
    CHECK(fclose(f) == 0);
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

/* A fresh, empty problem directory. Returns a static buffer. */
static const char *new_root(void)
{
    static char buf[256];
    static int n;

    snprintf(buf, sizeof buf, TMP "/r%d", ++n);
    make_dir(buf);
    return buf;
}

/* Creates root/name, with problem.md holding `md` unless it is NULL. */
static void add_pack(const char *root, const char *name, const char *md)
{
    char path[512];

    snprintf(path, sizeof path, "%s/%s", root, name);
    make_dir(path);
    if (md) {
        snprintf(path, sizeof path, "%s/%s/problem.md", root, name);
        write_file(path, md);
    }
}

static lc_ctx *open_dir(const char *dir)
{
    lc_options opt;

    lc_options_init(&opt);
    opt.problems_dir = dir;
    return lc_open(&opt);
}

static lc_ctx *load_dir(const char *dir)
{
    lc_ctx *ctx = open_dir(dir);

    CHECK_INT(lc_load(ctx), LC_OK);
    return ctx;
}

static const char *pdir(const lc_ctx *ctx)
{
    const char *d = lc_problems_dir(ctx);
    return d ? d : "(none)";
}

/* A problem directory holding only `name` must load nothing and record
 * "<problems_dir>/<name>/problem.md<suffix>". */
static void expect_bad(const char *name, const char *md, const char *suffix)
{
    const char *root = new_root();
    char want[1024];
    lc_ctx *ctx;

    add_pack(root, name, md);
    ctx = load_dir(root);
    snprintf(want, sizeof want, "%s/%s/problem.md%s", pdir(ctx), name, suffix);
    CHECK_INT(lc_problem_count(ctx), 0);
    CHECK_INT(lc_warning_count(ctx), 1);
    CHECK_STR(lc_warning_at(ctx, 0), want);
    lc_close(ctx);
}

/* Loads a problem directory holding one valid pack. Close *ctxp afterwards. */
static const lc_problem *load_good(lc_ctx **ctxp, const char *name,
                                   const char *md)
{
    const char *root = new_root();

    add_pack(root, name, md);
    *ctxp = load_dir(root);
    CHECK_INT(lc_warning_count(*ctxp), 0);
    if (lc_warning_count(*ctxp))
        fprintf(stderr, "    unexpected warning: %s\n", lc_warning_at(*ctxp, 0));
    CHECK_INT(lc_problem_count(*ctxp), 1);
    return lc_problem_at(*ctxp, 0);
}

/* Checks the argument names of a one-pack directory. */
static void expect_args(const char *md, const char *const *want, size_t n)
{
    lc_ctx *ctx;
    const lc_problem *p = load_good(&ctx, "0001-a", md);
    size_t i;

    if (p) {
        CHECK_INT(p->narg_names, n);
        CHECK(p->arg_names != NULL);
        for (i = 0; i < n && i < p->narg_names; i++)
            CHECK_STR(p->arg_names[i], want[i]);
    }
    lc_close(ctx);
}

/* ------------------------------------------------------------------ */

static void check_loaded_from(const char *opt, const char *want_dir,
                              const char *want_slug)
{
    lc_ctx *ctx = open_dir(opt);

    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_STR(lc_problems_dir(ctx), want_dir);
    CHECK_INT(lc_problem_count(ctx), 1);
    if (lc_problem_count(ctx) == 1)
        CHECK_STR(lc_problem_at(ctx, 0)->slug, want_slug);
    lc_close(ctx);
}

/* With no usable directory left, lc_load falls back to LC_DATADIR. */
static void check_fallback(const char *xdg_tried)
{
    lc_arena a = {0};
    lc_ctx *ctx = open_dir(NULL);
    char want[2048];
    int rc = lc_load(ctx);

    if (lc_is_dir(LC_DATADIR "/problems")) {
        /* Installed on this machine: it is used. */
        CHECK_INT(rc, LC_OK);
        CHECK_STR(lc_problems_dir(ctx), lc_realpath(&a, LC_DATADIR "/problems"));
    } else {
        if (xdg_tried)
            snprintf(want, sizeof want, "no problem directory found (tried "
                     "'%s' and '%s')%s", xdg_tried, LC_DATADIR "/problems",
                     not_found_hint);
        else
            snprintf(want, sizeof want, "no problem directory found (tried "
                     "'%s')%s", LC_DATADIR "/problems", not_found_hint);
        CHECK_INT(rc, LC_ERR_NOTFOUND);
        CHECK_STR(lc_last_error(ctx), want);
        CHECK(lc_problems_dir(ctx) == NULL);
        CHECK_INT(lc_problem_count(ctx), 0);
    }
    lc_close(ctx);
    lc_arena_free(&a);
}

/* lc_load fails with `status` and a message starting with `prefix`. */
static void check_load_error(const char *opt, int status, const char *prefix)
{
    lc_ctx *ctx = open_dir(opt);

    CHECK_INT(lc_load(ctx), status);
    CHECK(strncmp(lc_last_error(ctx), prefix, strlen(prefix)) == 0);
    if (strncmp(lc_last_error(ctx), prefix, strlen(prefix)) != 0)
        fprintf(stderr, "    got: %s\n    want prefix: %s\n",
                lc_last_error(ctx), prefix);
    CHECK(lc_problems_dir(ctx) == NULL);
    CHECK_INT(lc_problem_count(ctx), 0);
    lc_close(ctx);
}

static void test_search_order(void)
{
    lc_arena a = {0};
    char *base, *opt, *env, *xdg, *xdg_dir, *home, *home_dir, *missing;
    lc_ctx *ctx;

    add_pack(TMP "/order/opt", "0001-opt", HDR("1", "opt") "---\n");
    add_pack(TMP "/order/env", "0002-env", HDR("2", "env") "---\n");
    add_pack(TMP "/order/xdg/lcclone/problems", "0003-xdg",
             HDR("3", "xdg") "---\n");
    add_pack(TMP "/order/home/.local/share/lcclone/problems", "0004-home",
             HDR("4", "home") "---\n");
    write_file(TMP "/order/file", "not a directory\n");
    base = lc_realpath(&a, TMP "/order");
    CHECK(base != NULL);
    if (!base)
        return;
    opt = lc_path_join(&a, base, "opt");
    env = lc_path_join(&a, base, "env");
    xdg = lc_path_join(&a, base, "xdg");
    xdg_dir = lc_path_join(&a, xdg, "lcclone/problems");
    home = lc_path_join(&a, base, "home");
    home_dir = lc_path_join(&a, home, ".local/share/lcclone/problems");
    missing = lc_path_join(&a, base, "missing");

    setenv("LC_PROBLEMS", env, 1);
    setenv("XDG_DATA_HOME", xdg, 1);
    setenv("HOME", home, 1);

    /* 1. lc_options.problems_dir beats everything; relative is fine. */
    check_loaded_from(TMP "/order/opt", opt, "opt");
    /* 2. Then $LC_PROBLEMS. */
    check_loaded_from(NULL, env, "env");
    /* An empty LC_PROBLEMS counts as unset. */
    setenv("LC_PROBLEMS", "", 1);
    check_loaded_from(NULL, xdg_dir, "xdg");
    /* 3. Then $XDG_DATA_HOME/lcclone/problems. */
    unsetenv("LC_PROBLEMS");
    check_loaded_from(NULL, xdg_dir, "xdg");
    /* ... which defaults to ~/.local/share/lcclone/problems. */
    unsetenv("XDG_DATA_HOME");
    check_loaded_from(NULL, home_dir, "home");
    setenv("XDG_DATA_HOME", "relative/dirs/are/ignored", 1);
    check_loaded_from(NULL, home_dir, "home");
    /* 4. An XDG_DATA_HOME without the directory skips HOME's default. */
    setenv("XDG_DATA_HOME", missing, 1);
    check_fallback(lc_path_join(&a, missing, "lcclone/problems"));
    /* No HOME and no XDG_DATA_HOME: only LC_DATADIR is tried. */
    unsetenv("XDG_DATA_HOME");
    unsetenv("HOME");
    check_fallback(NULL);
    /* XDG_DATA_HOME naming a file is skipped like a missing one. */
    setenv("XDG_DATA_HOME", lc_path_join(&a, base, "file"), 1);
    check_fallback(lc_path_join(&a, base, "file/lcclone/problems"));

    /* A directory named by the option or LC_PROBLEMS must exist. */
    setenv("XDG_DATA_HOME", xdg, 1);
    check_load_error(TMP "/order/missing", LC_ERR_NOTFOUND,
                     "cannot use problem directory '" TMP "/order/missing': ");
    check_load_error(TMP "/order/file", LC_ERR_NOTFOUND,
                     "cannot use problem directory '" TMP "/order/file': ");
    check_load_error("", LC_ERR_NOTFOUND, "cannot use problem directory '': ");
    setenv("LC_PROBLEMS", TMP "/order/missing", 1);
    check_load_error(NULL, LC_ERR_NOTFOUND,
                     "cannot use LC_PROBLEMS directory '" TMP "/order/missing': ");
    setenv("LC_PROBLEMS", TMP "/order/file", 1);
    check_load_error(NULL, LC_ERR_NOTFOUND,
                     "cannot use LC_PROBLEMS directory '" TMP "/order/file': ");
    /* The option still wins over a bad LC_PROBLEMS. */
    check_loaded_from(TMP "/order/opt", opt, "opt");

    /* A failed lc_load can be retried once the problem is fixed. */
    ctx = open_dir(NULL);
    CHECK_INT(lc_load(ctx), LC_ERR_NOTFOUND);
    setenv("LC_PROBLEMS", env, 1);
    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_STR(lc_problems_dir(ctx), env);
    lc_close(ctx);

    unsetenv("LC_PROBLEMS");
    unsetenv("XDG_DATA_HOME");
    unsetenv("HOME");
    lc_arena_free(&a);
}

static void test_empty_and_ignored(void)
{
    lc_arena a = {0};
    const char *root = new_root();
    char path[512], target[512];
    lc_ctx *ctx;

    /* An empty directory is fine. */
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 0);
    CHECK_INT(lc_warning_count(ctx), 0);
    CHECK(lc_problem_at(ctx, 0) == NULL);
    CHECK_STR(lc_problems_dir(ctx), lc_realpath(&a, root));
    CHECK(lc_problems_dir(ctx)[0] == '/');
    lc_close(ctx);

    /* Only directories whose names start with a digit are packs. */
    snprintf(path, sizeof path, "%s/0002-file", root);
    write_file(path, HDR("2", "file") "---\n");
    snprintf(path, sizeof path, "%s/MANIFEST.tsv", root);
    write_file(path, "id\tslug\n");
    add_pack(root, "abc", HDR("3", "abc") "---\n");
    add_pack(root, ".0004-dot", HDR("4", "dot") "---\n");
    add_pack(root, "_0005-underscore", "not a header\n");
    add_pack(root, "x0006-letter", "not a header\n");
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 0);
    CHECK_INT(lc_warning_count(ctx), 0);
    lc_close(ctx);

    /* A symbolic link to a pack directory is followed. */
    add_pack(TMP "/elsewhere", "pack", HDR("7", "linked") "---\n");
    snprintf(target, sizeof target, "%s/pack", lc_realpath(&a, TMP "/elsewhere"));
    snprintf(path, sizeof path, "%s/0007-linked", root);
    CHECK(symlink(target, path) == 0);
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 1);
    CHECK_INT(lc_warning_count(ctx), 0);
    if (lc_problem_count(ctx) == 1) {
        snprintf(path, sizeof path, "%s/0007-linked", lc_problems_dir(ctx));
        CHECK_STR(lc_problem_at(ctx, 0)->dir, path);
    }
    lc_close(ctx);
    lc_arena_free(&a);
}

static void test_double_load(void)
{
    lc_ctx *ctx = load_dir(FIXTURES);
    size_t n = lc_problem_count(ctx);

    CHECK(n > 0);
    CHECK_INT(lc_load(ctx), LC_ERR_INVALID);
    CHECK_STR(lc_last_error(ctx), "problems are already loaded");
    CHECK_INT(lc_problem_count(ctx), n);
    CHECK(lc_problem_find(ctx, "two-sum") != NULL);
    lc_close(ctx);
}

static void check_strings(const char *const *got, size_t n,
                          const char *const *want, size_t nwant)
{
    size_t i;

    CHECK_INT(n, nwant);
    CHECK(got != NULL);
    for (i = 0; got && i < n && i < nwant; i++)
        CHECK_STR(got[i], want[i]);
}

static void test_fixture_tree(void)
{
    static const int ids[] = {1, 4, 141, 155, 650, 2667};
    static const char *const two_sum_tags[] = {"array", "hash-table"};
    static const char *const two_sum_args[] = {"nums", "target"};
    static const char *const median_args[] = {"nums1", "nums2"};
    static const char *const cycle_args[] = {"head", "pos"};
    static const char *const keys_args[] = {"n"};
    lc_arena a = {0};
    lc_ctx *ctx = load_dir(FIXTURES);
    const char *abs = lc_realpath(&a, FIXTURES);
    const lc_problem *p;
    char want[512];
    size_t i;

    CHECK_STR(lc_problems_dir(ctx), abs);
    CHECK_INT(lc_warning_count(ctx), 0);
    CHECK_INT(lc_problem_count(ctx), 6);
    for (i = 0; i < 6 && i < lc_problem_count(ctx); i++)
        CHECK_INT(lc_problem_at(ctx, i)->id, ids[i]);

    p = lc_problem_find(ctx, "two-sum");
    CHECK(p != NULL);
    if (p) {
        CHECK_INT(p->id, 1);
        CHECK_STR(p->slug, "two-sum");
        CHECK_STR(p->title, "Two Sum");
        CHECK_INT(p->difficulty, LC_EASY);
        check_strings(p->tags, p->ntags, two_sum_tags, 2);
        CHECK_STR(p->signature, "int[] twoSum(int[] nums, int target)");
        check_strings(p->arg_names, p->narg_names, two_sum_args, 2);
        CHECK_INT(p->time_limit_ms, 1000);
        CHECK_INT(p->memory_limit_mb, 256);
        snprintf(want, sizeof want, "%s/0001-two-sum", abs);
        CHECK_STR(p->dir, want);
        CHECK(strncmp(p->statement, "You are given an array of integers "
                      "`nums`", 41) == 0);
        CHECK_INT(p->statement_len, strlen(p->statement));
        CHECK(strstr(p->statement, "\n@examples\n") != NULL);
        CHECK_INT(p->statement_line, 12);
        /* Comment and blank lines are not fields. */
        CHECK_INT(p->nfields, 7);
        if (p->nfields == 7) {
            CHECK_STR(p->fields[0].key, "id");
            CHECK_STR(p->fields[0].value, "1");
            CHECK_INT(p->fields[0].line, 2);
            CHECK_STR(p->fields[4].key, "tags");
            CHECK_INT(p->fields[4].line, 8);
            CHECK_STR(p->fields[6].key, "compare");
            CHECK_STR(p->fields[6].value, "unordered 0");
            CHECK_INT(p->fields[6].line, 10);
        }
        CHECK_STR(lc_problem_field(p, "compare"), "unordered 0");
    }

    p = lc_problem_find(ctx, "4");
    CHECK(p != NULL);
    if (p) {
        CHECK_STR(p->slug, "median-of-two-sorted-arrays");
        CHECK_INT(p->difficulty, LC_HARD);
        CHECK_INT(p->ntags, 3);
        check_strings(p->arg_names, p->narg_names, median_args, 2);
        CHECK(strstr(p->statement, "\n> Aim for") != NULL);
    }

    p = lc_problem_find(ctx, "linked-list-cycle");
    CHECK(p != NULL);
    if (p) {
        CHECK_STR(p->signature, "bool hasCycle(ListNode head)");
        CHECK_STR(lc_problem_field(p, "hidden"), "int pos");
        check_strings(p->arg_names, p->narg_names, cycle_args, 2);
    }

    p = lc_problem_find(ctx, "min-stack");
    CHECK(p != NULL);
    if (p) {
        CHECK_INT(p->difficulty, LC_MEDIUM);
        CHECK(p->signature == NULL);
        CHECK_INT(p->narg_names, 0);
        CHECK(p->arg_names != NULL);
        CHECK_STR(lc_problem_field(p, "class"), "MinStack");
        CHECK(lc_problem_field(p, "signature") == NULL);
        /* The statement's own '---' is a thematic break, not a separator. */
        CHECK(strstr(p->statement, "\n---\n") != NULL);
        CHECK(strstr(p->statement, "@examples") == NULL);
    }

    /* 0650 has a byte order mark and CRLF line endings. */
    p = lc_problem_find(ctx, "2-keys-keyboard");
    CHECK(p != NULL);
    if (p) {
        CHECK_INT(p->id, 650);
        CHECK_STR(p->title, "2 Keys Keyboard");
        CHECK_INT(p->difficulty, LC_MEDIUM);
        CHECK_INT(p->time_limit_ms, 2000);
        CHECK_INT(p->memory_limit_mb, 64);
        check_strings(p->arg_names, p->narg_names, keys_args, 1);
        CHECK(p->nfields > 0 && strcmp(p->fields[0].key, "id") == 0);
        for (i = 0; i < p->nfields; i++)
            CHECK(strchr(p->fields[i].value, '\r') == NULL);
        CHECK(strchr(p->statement, '\r') == NULL);
        CHECK_INT(p->statement_len, strlen(p->statement));
        CHECK(strncmp(p->statement, "A text box starts", 17) == 0);
        CHECK_INT(p->statement_line, 11);
        CHECK(strstr(p->statement, "@examples") == NULL);
    }

    p = lc_problem_find(ctx, "2667");
    CHECK(p != NULL);
    if (p) {
        CHECK_STR(p->signature, "string helloWorld()");
        CHECK_INT(p->narg_names, 0);
        CHECK(p->arg_names != NULL);
        CHECK_INT(p->ntags, 0);
        CHECK(p->tags != NULL);
        CHECK(lc_problem_field(p, "tags") == NULL);
    }

    /* Non-pack entries: MANIFEST.tsv, _template, .draft, 0000-notes.txt. */
    CHECK(lc_problem_find(ctx, "your-slug") == NULL);
    CHECK(lc_problem_find(ctx, "hidden-draft") == NULL);
    lc_close(ctx);
    lc_arena_free(&a);
}

static void test_bad_tree(void)
{
    static const char *const warnings[] = {
        "0001-beta/problem.md:1: duplicate id 1 (already used by 0001-alpha)",
        "0002-missing-separator/problem.md: missing '---' line after the header",
        "0003-missing-id/problem.md: missing required field 'id'",
        "0004-missing-slug/problem.md: missing required field 'slug'",
        "0005-missing-title/problem.md: missing required field 'title'",
        "0006-missing-difficulty/problem.md: missing required field 'difficulty'",
        "0007-bad-difficulty/problem.md:4: unknown difficulty 'eazy'",
        "0008-bad-slug/problem.md:2: invalid slug 'bad--slug' (expected "
        "lowercase letters and digits joined by single dashes)",
        "0009-bad-id/problem.md:1: invalid id 'nine' (expected an integer from "
        "1 to 99999)",
        "0010-dir-mismatch/problem.md: directory name '0010-dir-mismatch' does "
        "not match id and slug (expected '0010-other-name')",
        "0011-duplicate-key/problem.md:5: duplicate key 'title' (first on line 3)",
        "0012-alpha/problem.md:2: duplicate slug 'alpha' (already used by "
        "0001-alpha)",
        "0013-bad-tags/problem.md:5: invalid tag 'Hash Table' (use lowercase "
        "letters, digits and dashes)",
        "0014-empty-tag/problem.md:5: empty tag in 'array,,math'",
        "0015-bad-time-limit/problem.md:5: invalid time_limit_ms '0' (expected "
        "a positive integer)",
        "0016-bad-memory-limit/problem.md:5: invalid memory_limit_mb '64MB' "
        "(expected a positive integer)",
        "0017-missing-problem-md/problem.md: file not found",
        "0018-signature-no-parens/problem.md:5: signature has no parentheses",
        "0019-signature-unnamed-param/problem.md:5: signature: parameter 'int' "
        "has no name",
        "0020-bad-hidden/problem.md:6: hidden: parameter 'int' has no name",
        "0021-bad-key/problem.md:3: invalid key 'Title'",
        "0022-bad-header-line/problem.md:5: expected 'key: value'",
        "0023-empty-title/problem.md:3: empty title",
    };
    static const int ids[] = {1, 9000, 10000};
    static const char *const slugs[] = {"alpha", "delta", "epsilon"};
    size_t nw = sizeof warnings / sizeof warnings[0], i;
    lc_ctx *ctx = load_dir(BAD);
    char want[1024];

    CHECK_INT(lc_warning_count(ctx), nw);
    for (i = 0; i < nw && i < lc_warning_count(ctx); i++) {
        snprintf(want, sizeof want, "%s/%s", pdir(ctx), warnings[i]);
        CHECK_STR(lc_warning_at(ctx, i), want);
    }
    CHECK(lc_warning_at(ctx, nw) == NULL);
    CHECK_INT(lc_problem_count(ctx), 3);
    for (i = 0; i < 3 && i < lc_problem_count(ctx); i++) {
        CHECK_INT(lc_problem_at(ctx, i)->id, ids[i]);
        CHECK_STR(lc_problem_at(ctx, i)->slug, slugs[i]);
    }
    lc_close(ctx);
}

static void test_header_syntax(void)
{
    lc_ctx *ctx;
    const lc_problem *p;
    char md[512];
    size_t i;

    /* Comments, blank lines, trimming, keys with digits and '_'. */
    p = load_good(&ctx, "0001-a",
                  "# a comment\n"
                  "id: 1\n"
                  "\n"
                  " \t \n"
                  "slug:a\n"
                  "title: \t Two  Words \t\n"
                  "#another: comment\n"
                  "difficulty: easy\n"
                  "note:\n"
                  "x_2: has: colons\n"
                  "_private: yes\n"
                  "--- \t \n"
                  "Body\n");
    if (p) {
        CHECK_STR(p->slug, "a");
        CHECK_STR(p->title, "Two  Words");
        CHECK_INT(p->nfields, 7);
        if (p->nfields == 7) {
            CHECK_INT(p->fields[0].line, 2);
            CHECK_STR(p->fields[1].key, "slug");
            CHECK_INT(p->fields[1].line, 5);
            CHECK_STR(p->fields[4].key, "note");
            CHECK_STR(p->fields[4].value, "");
            CHECK_INT(p->fields[4].line, 9);
            CHECK_STR(p->fields[5].value, "has: colons");
            CHECK_STR(p->fields[6].key, "_private");
        }
        CHECK_STR(lc_problem_field(p, "note"), "");
        CHECK_STR(lc_problem_field(p, "x_2"), "has: colons");
        CHECK_STR(p->statement, "Body\n");
        CHECK_INT(p->statement_line, 13);
    }
    lc_close(ctx);

    /* Bad keys and lines. */
    expect_bad("0001-a", HDR("1", "a") "Title: X\n---\n",
               ":5: invalid key 'Title'");
    expect_bad("0001-a", HDR("1", "a") "my-key: x\n---\n",
               ":5: invalid key 'my-key'");
    expect_bad("0001-a", HDR("1", "a") "1st: x\n---\n", ":5: invalid key '1st'");
    expect_bad("0001-a", HDR("1", "a") ": x\n---\n", ":5: invalid key ''");
    expect_bad("0001-a", HDR("1", "a") " note: x\n---\n",
               ":5: invalid key ' note'");
    expect_bad("0001-a", HDR("1", "a") "note : x\n---\n",
               ":5: invalid key 'note '");
    expect_bad("0001-a", HDR("1", "a") "just words\n---\n",
               ":5: expected 'key: value'");
    expect_bad("0001-a", HDR("1", "a") "  # indented\n---\n",
               ":5: expected 'key: value'");
    expect_bad("0001-a", HDR("1", "a") "----\n---\n",
               ":5: expected 'key: value'");
    expect_bad("0001-a", "id: 1\r\nslug: a\r\nBAD\r\n---\r\n",
               ":3: expected 'key: value'");

    /* The separator: exactly "---" at the start, then spaces and tabs. */
    expect_bad("0001-a", HDR("1", "a") " ---\nx\n",
               ": missing '---' line after the header");
    expect_bad("0001-a", HDR("1", "a") "--- x\n",
               ": missing '---' line after the header");
    expect_bad("0001-a", HDR("1", "a") "-- -\n",
               ": missing '---' line after the header");
    expect_bad("0001-a", "", ": missing '---' line after the header");
    /* Without a separator, statement lines are not blamed. */
    expect_bad("0001-a", HDR("1", "a") "A statement line.\n",
               ": missing '---' line after the header");

    /* Duplicate keys, known or not; the earliest repeat is reported. */
    expect_bad("0001-a", HDR("1", "a") "id: 1\n---\n",
               ":5: duplicate key 'id' (first on line 1)");
    expect_bad("0001-a", HDR("1", "a") "x: 1\ny: 2\ny: 3\nx: 4\n---\n",
               ":7: duplicate key 'y' (first on line 6)");
    expect_bad("0001-a", HDR("1", "a") "x: 1\ny: 2\nx: 3\ny: 4\nx: 5\n---\n",
               ":7: duplicate key 'x' (first on line 5)");
    expect_bad("0001-a", HDR("1", "a") "note:\nnote: again\n---\n",
               ":6: duplicate key 'note' (first on line 5)");

    /* Long values are shortened in messages, at a UTF-8 boundary. */
    memset(md, 0, sizeof md);
    strcpy(md, "id: 1\nslug: ");
    for (i = 0; i < 100; i++)
        strcat(md, "X");
    strcat(md, "\ntitle: T\ndifficulty: easy\n---\n");
    expect_bad("0001-a", md, ":2: invalid slug 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
               "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX...' (expected lowercase letters "
               "and digits joined by single dashes)");
    strcpy(md, "id: 1\nslug: ");
    for (i = 0; i < 59; i++)
        strcat(md, "a");
    strcat(md, "\xC3\xA9xyz\ntitle: T\ndifficulty: easy\n---\n");
    expect_bad("0001-a", md, ":2: invalid slug 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
               "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa...' (expected lowercase letters "
               "and digits joined by single dashes)");
}

static void test_bom_crlf(void)
{
    lc_ctx *ctx;
    const lc_problem *p;

    p = load_good(&ctx, "0001-a",
                  "\xEF\xBB\xBFid: 1\r\n"
                  "slug: a \r\n"
                  "title: Crlf\r\n"
                  "# comment\r\n"
                  "\r\n"
                  "difficulty: hard\r\n"
                  "signature: int f(int x, int y)\r\n"
                  "---\r\n"
                  "Line one\r\n"
                  "\r\n"
                  "a\rb\r\n"
                  "last");
    if (p) {
        CHECK_INT(p->id, 1);
        CHECK_STR(p->slug, "a");
        CHECK_STR(p->title, "Crlf");
        CHECK_INT(p->difficulty, LC_HARD);
        CHECK_INT(p->narg_names, 2);
        CHECK_STR(p->fields[0].key, "id");
        CHECK_INT(p->fields[3].line, 6);
        /* A '\r' is dropped only before a newline. */
        CHECK_STR(p->statement, "Line one\n\na\rb\nlast");
        CHECK_INT(p->statement_len, 18);
        CHECK_INT(p->statement_line, 9);
    }
    lc_close(ctx);

    /* The loader and the markdown parser agree on line numbers, even with
     * a lone '\r': statement_line + block line - 1 is the line in
     * problem.md (docs/json.md). '## Heading' is on line 8. */
    p = load_good(&ctx, "0001-a", HDR("1", "a") "---\n"
                  "first para\rsecond line\n\n## Heading\n");
    if (p) {
        lc_md_doc *doc = NULL;

        CHECK_INT(p->statement_line, 6);
        CHECK_INT(lc_md_parse(p->statement, p->statement_len, &doc), LC_OK);
        CHECK(doc && doc->nblocks == 2);
        if (doc && doc->nblocks == 2) {
            CHECK_INT(doc->blocks[0].kind, LC_MD_PARAGRAPH);
            CHECK_INT(p->statement_line + doc->blocks[0].line - 1, 6);
            CHECK_INT(doc->blocks[1].kind, LC_MD_HEADING);
            CHECK_INT(p->statement_line + doc->blocks[1].line - 1, 8);
        }
        lc_md_free(doc);
    }
    lc_close(ctx);

    /* A BOM is skipped only at the very start. */
    expect_bad("0001-a", "id: 1\n\xEF\xBB\xBFslug: a\n---\n",
               ":2: invalid key '\xEF\xBB\xBFslug'");
}

/* A NUL byte would cut the strings, so the pack is skipped. */
static void test_nul_byte(void)
{
    static const char md[] = "id: 1\nslug: a\ntitle: T\ndifficulty: easy\n"
                             "---\nText\0more\n";
    const char *root = new_root();
    char path[512], want[1024];
    lc_ctx *ctx;
    FILE *f;

    add_pack(root, "0001-a", NULL);
    snprintf(path, sizeof path, "%s/0001-a/problem.md", root);
    f = fopen(path, "wb");
    CHECK(f != NULL);
    if (!f)
        return;
    CHECK(fwrite(md, 1, sizeof md - 1, f) == sizeof md - 1);
    CHECK(fclose(f) == 0);
    ctx = load_dir(root);
    snprintf(want, sizeof want, "%s/0001-a/problem.md:6: contains a NUL byte",
             pdir(ctx));
    CHECK_INT(lc_problem_count(ctx), 0);
    CHECK_INT(lc_warning_count(ctx), 1);
    CHECK_STR(lc_warning_at(ctx, 0), want);
    lc_close(ctx);
}

static void test_required_fields(void)
{
    expect_bad("0001-a", "---\n", ": missing required field 'id'");
    expect_bad("0001-a", "slug: a\ntitle: T\ndifficulty: easy\n---\n",
               ": missing required field 'id'");
    expect_bad("0001-a", "id: 1\ntitle: T\ndifficulty: easy\n---\n",
               ": missing required field 'slug'");
    expect_bad("0001-a", "id: 1\nslug: a\ndifficulty: easy\n---\n",
               ": missing required field 'title'");
    expect_bad("0001-a", "id: 1\nslug: a\ntitle: T\n---\n",
               ": missing required field 'difficulty'");
    /* A field after the separator does not count. */
    expect_bad("0001-a", "id: 1\nslug: a\ntitle: T\n---\ndifficulty: easy\n",
               ": missing required field 'difficulty'");
}

static void test_id(void)
{
    static const char *const bad[] = {
        "0", "100000", "-1", "+1", "1.0", "0x1", "1e3", "", "1 2", "one",
        "99999999999999999999999999"
    };
    lc_ctx *ctx;
    const lc_problem *p;
    char md[256], suffix[256];
    size_t i;

    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        snprintf(md, sizeof md, "id: %s\nslug: a\ntitle: T\ndifficulty: easy\n"
                 "---\n", bad[i]);
        snprintf(suffix, sizeof suffix, ":1: invalid id '%s' (expected an "
                 "integer from 1 to 99999)", bad[i]);
        expect_bad("0001-a", md, suffix);
    }
    /* Spaces around a value are trimmed, so this is the empty value. */
    expect_bad("0001-a", "id:   \nslug: a\ntitle: T\ndifficulty: easy\n---\n",
               ":1: invalid id '' (expected an integer from 1 to 99999)");

    p = load_good(&ctx, "99999-a", HDR("99999", "a") "---\n");
    if (p)
        CHECK_INT(p->id, 99999);
    lc_close(ctx);
    p = load_good(&ctx, "0007-a", HDR("007", "a") "---\n");
    if (p)
        CHECK_INT(p->id, 7);
    lc_close(ctx);
    p = load_good(&ctx, "0001-a", HDR("000000000000000000001", "a") "---\n");
    if (p)
        CHECK_INT(p->id, 1);
    lc_close(ctx);
    p = load_good(&ctx, "12345-a", HDR("12345", "a") "---\n");
    if (p)
        CHECK_INT(p->id, 12345);
    lc_close(ctx);
}

static void test_slug_title_difficulty(void)
{
    static const char *const bad_slugs[] = {
        "Two-Sum", "two_sum", "two--sum", "-two", "two-", "-", "", "two sum",
        "two.sum", "caf\xC3\xA9"
    };
    static const char *const good_slugs[] = {
        "3sum", "2-keys-keyboard", "a", "a1-b2-c3", "1"
    };
    static const char *const bad_diff[] = {"Easy", "EASY", "eazy", "", "easy!"};
    static const char *const diffs[] = {"easy", "medium", "hard"};
    static const lc_difficulty dvals[] = {LC_EASY, LC_MEDIUM, LC_HARD};
    lc_ctx *ctx;
    const lc_problem *p;
    char md[256], name[128], suffix[256];
    size_t i;

    for (i = 0; i < sizeof bad_slugs / sizeof bad_slugs[0]; i++) {
        snprintf(md, sizeof md, "id: 1\nslug: %s\ntitle: T\ndifficulty: easy\n"
                 "---\n", bad_slugs[i]);
        snprintf(suffix, sizeof suffix, ":2: invalid slug '%s' (expected "
                 "lowercase letters and digits joined by single dashes)",
                 bad_slugs[i]);
        expect_bad("0001-a", md, suffix);
    }
    for (i = 0; i < sizeof good_slugs / sizeof good_slugs[0]; i++) {
        snprintf(md, sizeof md, HDR("1", "%s") "---\n", good_slugs[i]);
        snprintf(name, sizeof name, "0001-%s", good_slugs[i]);
        p = load_good(&ctx, name, md);
        if (p)
            CHECK_STR(p->slug, good_slugs[i]);
        lc_close(ctx);
    }

    expect_bad("0001-a", "id: 1\nslug: a\ntitle:\ndifficulty: easy\n---\n",
               ":3: empty title");
    expect_bad("0001-a", "id: 1\nslug: a\ntitle: \t \ndifficulty: easy\n---\n",
               ":3: empty title");

    for (i = 0; i < sizeof bad_diff / sizeof bad_diff[0]; i++) {
        snprintf(md, sizeof md, "id: 1\nslug: a\ntitle: T\ndifficulty: %s\n"
                 "---\n", bad_diff[i]);
        snprintf(suffix, sizeof suffix, ":4: unknown difficulty '%s'",
                 bad_diff[i]);
        expect_bad("0001-a", md, suffix);
    }
    for (i = 0; i < 3; i++) {
        snprintf(md, sizeof md, "id: 1\nslug: a\ntitle: T\ndifficulty: %s\n"
                 "---\n", diffs[i]);
        p = load_good(&ctx, "0001-a", md);
        if (p)
            CHECK_INT(p->difficulty, dvals[i]);
        lc_close(ctx);
    }
}

static void test_tags(void)
{
    static const char *const three[] = {"a", "b", "c-d"};
    static const char *const odd[] = {"-x-", "9", "a--b"};
    lc_ctx *ctx;
    const lc_problem *p;

    p = load_good(&ctx, "0001-a", HDR("1", "a") "tags:\n---\n");
    if (p) {
        CHECK_INT(p->ntags, 0);
        CHECK(p->tags != NULL);
        CHECK_STR(lc_problem_field(p, "tags"), "");
    }
    lc_close(ctx);
    p = load_good(&ctx, "0001-a", HDR("1", "a") "tags:  a ,b,\tc-d  \n---\n");
    if (p)
        check_strings(p->tags, p->ntags, three, 3);
    lc_close(ctx);
    /* Dashes may go anywhere in a tag. */
    p = load_good(&ctx, "0001-a", HDR("1", "a") "tags: -x-, 9, a--b\n---\n");
    if (p)
        check_strings(p->tags, p->ntags, odd, 3);
    lc_close(ctx);

    expect_bad("0001-a", HDR("1", "a") "tags: a,,b\n---\n",
               ":5: empty tag in 'a,,b'");
    expect_bad("0001-a", HDR("1", "a") "tags: a,\n---\n",
               ":5: empty tag in 'a,'");
    expect_bad("0001-a", HDR("1", "a") "tags: ,a\n---\n",
               ":5: empty tag in ',a'");
    expect_bad("0001-a", HDR("1", "a") "tags: ,\n---\n", ":5: empty tag in ','");
    expect_bad("0001-a", HDR("1", "a") "tags: a, \t,b\n---\n",
               ":5: empty tag in 'a, \t,b'");
    expect_bad("0001-a", HDR("1", "a") "tags: Hash\n---\n",
               ":5: invalid tag 'Hash' (use lowercase letters, digits and dashes)");
    expect_bad("0001-a", HDR("1", "a") "tags: a, hash table\n---\n",
               ":5: invalid tag 'hash table' (use lowercase letters, digits and "
               "dashes)");
    expect_bad("0001-a", HDR("1", "a") "tags: a_b\n---\n",
               ":5: invalid tag 'a_b' (use lowercase letters, digits and dashes)");
}

static void test_signature_and_hidden(void)
{
    static const char *const nums_target[] = {"nums", "target"};
    static const char *const with_hidden[] = {"headA", "headB", "skipA", "skipB"};
    static const char *const one[] = {"x"};
    static const char *const odd[] = {"nums", "x9", "z"};

    expect_args(HDR("1", "a") "signature: int[] twoSum(int[] nums, int target)\n"
                "---\n", nums_target, 2);
    /* Types come from the type table; a comma inside <> stays in the
     * type named in the message. */
    expect_bad("0001-a", HDR("1", "a") "signature: int f(map<int, string> m, "
               "int[][] grid)\n---\n",
               ":5: signature: unknown type 'map<int, string>'");
    expect_args(HDR("1", "a") "signature: string hello()\n---\n", NULL, 0);
    expect_args(HDR("1", "a") "signature: void f( \t )\n---\n", NULL, 0);
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int x) const\n---\n",
               ":5: signature: unexpected 'const' after ')'");
    /* The name is the identifier at the end: digits and '_' are fine, and
     * no space is needed after ']'. */
    expect_args(HDR("1", "a") "signature: int f(int[]nums, int x9, "
                "string[][]z)\n---\n", odd, 3);
    expect_bad("0001-a", HDR("1", "a") "signature: int f(vector<int>&nums)\n"
               "---\n", ":5: signature: unknown type 'vector<int>&'");
    /* hidden: names come after the signature's. */
    expect_args(HDR("1", "a") "signature: ListNode getIntersectionNode(ListNode "
                "headA, ListNode headB)\nhidden: int skipA, int skipB\n---\n",
                with_hidden, 4);
    expect_args(HDR("1", "a") "hidden: int skipA\nsignature: int f(int x)\n---\n",
                (const char *const[]){"x", "skipA"}, 2);
    expect_args(HDR("1", "a") "signature: int f(int x)\nhidden:\n---\n", one, 1);
    /* No names at all without a signature (lc.h), but hidden is checked. */
    expect_args(HDR("1", "a") "hidden: int pos\n---\n", NULL, 0);
    expect_bad("0001-a", HDR("1", "a") "hidden: int\n---\n",
               ":5: hidden: parameter 'int' has no name");

    expect_bad("0001-a", HDR("1", "a") "signature: int answer\n---\n",
               ":5: signature has no parentheses");
    expect_bad("0001-a", HDR("1", "a") "signature:\n---\n",
               ":5: signature has no parentheses");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int x\n---\n",
               ":5: signature has no parentheses");
    expect_bad("0001-a", HDR("1", "a") "signature: int f int x)\n---\n",
               ":5: signature has no parentheses");
    expect_bad("0001-a", HDR("1", "a") "signature: int f)int x(\n---\n",
               ":5: signature has no parentheses");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int)\n---\n",
               ":5: signature: parameter 'int' has no name");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int[] nums, int[])\n---\n",
               ":5: signature: parameter 'int[]' has no name");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(vector<int>)\n---\n",
               ":5: signature: parameter 'vector<int>' has no name");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int 9)\n---\n",
               ":5: signature: parameter 'int 9' has no name");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int a,)\n---\n",
               ":5: signature: parameter 2 is empty");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(, int a)\n---\n",
               ":5: signature: parameter 1 is empty");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int a, int b) \n"
               "hidden: int c,,int d\n---\n", ":6: hidden: parameter 2 is empty");
}

/* Checks a type against its kind and dims. */
static void check_type(lc_type t, lc_kind kind, int dims)
{
    CHECK_INT(t.kind, kind);
    CHECK_INT(t.dims, dims);
}

static void test_typed_fields(void)
{
    lc_ctx *ctx;
    const lc_problem *p;

    p = load_good(&ctx, "0001-a", HDR("1", "a") "signature: int[] twoSum("
                  "int[] nums, int target)\ncompare: unordered 0\n---\n");
    if (p) {
        CHECK(p->sig != NULL);
        if (p->sig) {
            check_type(p->sig->ret, LC_T_INT, 1);
            CHECK_STR(p->sig->name, "twoSum");
            CHECK_INT(p->sig->nparams, 2);
            if (p->sig->nparams == 2) {
                check_type(p->sig->params[0].type, LC_T_INT, 1);
                CHECK_STR(p->sig->params[0].name, "nums");
                check_type(p->sig->params[1].type, LC_T_INT, 0);
                CHECK_STR(p->sig->params[1].name, "target");
            }
        }
        CHECK_INT(p->nhidden, 0);
        CHECK(p->hidden != NULL);
        CHECK_INT(p->compare.mode, LC_CMP_UNORDERED);
        CHECK_INT(p->compare.levels, 1);
    }
    lc_close(ctx);

    p = load_good(&ctx, "0001-a", HDR("1", "a") "signature: bool hasCycle("
                  "ListNode head)\nhidden: int pos\n---\n");
    if (p) {
        CHECK(p->sig && p->sig->nparams == 1);
        if (p->sig)
            check_type(p->sig->ret, LC_T_BOOL, 0);
        CHECK_INT(p->nhidden, 1);
        if (p->nhidden == 1) {
            check_type(p->hidden[0].type, LC_T_INT, 0);
            CHECK_STR(p->hidden[0].name, "pos");
        }
        CHECK_INT(p->compare.mode, LC_CMP_EXACT);
    }
    lc_close(ctx);

    /* A result made of doubles defaults to a tolerance of 1e-5. */
    p = load_good(&ctx, "0001-a", HDR("1", "a") "signature: double[] f("
                  "int n)\n---\n");
    if (p) {
        CHECK_INT(p->compare.mode, LC_CMP_FLOAT);
        CHECK(p->compare.tolerance == 1e-5);
    }
    lc_close(ctx);
    p = load_good(&ctx, "0001-a", HDR("1", "a") "signature: double f(int n)\n"
                  "compare: exact\n---\n");
    if (p)
        CHECK_INT(p->compare.mode, LC_CMP_EXACT);
    lc_close(ctx);

    /* Without a signature there is no sig, but hidden and compare are
     * still parsed. */
    p = load_good(&ctx, "0001-a", HDR("1", "a") "hidden: TreeNode[] t\n"
                  "compare: float 0.5\n---\n");
    if (p) {
        CHECK(p->sig == NULL);
        CHECK_INT(p->nhidden, 1);
        if (p->nhidden == 1)
            check_type(p->hidden[0].type, LC_T_TREE, 1);
        CHECK_INT(p->compare.mode, LC_CMP_FLOAT);
        CHECK(p->compare.tolerance == 0.5);
    }
    lc_close(ctx);

    expect_bad("0001-a", HDR("1", "a") "signature: vector<int> f(int x)\n"
               "---\n", ":5: signature: unknown type 'vector<int>'");
    expect_bad("0001-a", HDR("1", "a") "signature: int (int x)\n---\n",
               ":5: signature: expected a return type and a function name "
               "before '('");
    expect_bad("0001-a", HDR("1", "a") "signature: f(int x)\n---\n",
               ":5: signature: expected a return type and a function name "
               "before '('");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(void x)\n---\n",
               ":5: signature: only a return type can be void");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(ListNode[][] x)\n"
               "---\n", ":5: signature: type 'ListNode[][]' is not supported "
               "(too many '[]')");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int[][][] x)\n"
               "---\n", ":5: signature: type 'int[][][]' is not supported "
               "(too many '[]')");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int x, long x)\n"
               "---\n", ":5: signature: duplicate parameter name 'x'");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int class)\n"
               "---\n", ":5: signature: 'class' can't be used as a name");
    expect_bad("0001-a", HDR("1", "a") "signature: int main(int x)\n---\n",
               ":5: signature: 'main' can't be used as a name");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int lc_x)\n---\n",
               ":5: signature: 'lc_x' can't be used as a name");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int _x)\n---\n",
               ":5: signature: '_x' can't be used as a name");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int x)\n"
               "hidden: int x\n---\n",
               ":6: hidden: 'x' is already a parameter of the signature");
    expect_bad("0001-a", HDR("1", "a") "hidden: void v\n---\n",
               ":5: hidden: only a return type can be void");

    expect_bad("0001-a", HDR("1", "a") "compare: fuzzy\n---\n",
               ":5: compare: unknown mode 'fuzzy' (expected exact, float, "
               "unordered or checker)");
    expect_bad("0001-a", HDR("1", "a") "compare:\n---\n",
               ":5: compare: empty value (expected exact, float, unordered or "
               "checker)");
    expect_bad("0001-a", HDR("1", "a") "signature: int f(int x)\n"
               "compare: float\n---\n",
               ":6: compare: float needs a result made of doubles, not int");
    expect_bad("0001-a", HDR("1", "a") "signature: int[] f(int x)\n"
               "compare: unordered 1\n---\n",
               ":6: compare: int[] has no level 1 (levels count its array "
               "nestings from 0)");
    expect_bad("0001-a", HDR("1", "a") "signature: void f(int[] x)\n"
               "compare: unordered 0\n---\n",
               ":6: compare: void has no level 0 (levels count its array "
               "nestings from 0)");
}

static void test_limits(void)
{
    static const char *const bad[] = {"0", "-5", "1.5", "", "2147483648", "1 0",
                                      "99999999999999999999", "1000ms"};
    lc_ctx *ctx;
    const lc_problem *p;
    char md[256], suffix[256];
    size_t i;

    p = load_good(&ctx, "0001-a", HDR("1", "a") "---\n");
    if (p) {
        CHECK_INT(p->time_limit_ms, 1000);
        CHECK_INT(p->memory_limit_mb, 256);
        CHECK(p->signature == NULL);
        CHECK_INT(p->ntags, 0);
    }
    lc_close(ctx);
    p = load_good(&ctx, "0001-a", HDR("1", "a") "memory_limit_mb: 0064\n"
                  "time_limit_ms: 2147483647\n---\n");
    if (p) {
        CHECK_INT(p->time_limit_ms, 2147483647);
        CHECK_INT(p->memory_limit_mb, 64);
    }
    lc_close(ctx);

    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        snprintf(md, sizeof md, HDR("1", "a") "time_limit_ms: %s\n---\n", bad[i]);
        snprintf(suffix, sizeof suffix, ":5: invalid time_limit_ms '%s' "
                 "(expected a positive integer)", bad[i]);
        expect_bad("0001-a", md, suffix);
        snprintf(md, sizeof md, HDR("1", "a") "memory_limit_mb: %s\n---\n", bad[i]);
        snprintf(suffix, sizeof suffix, ":5: invalid memory_limit_mb '%s' "
                 "(expected a positive integer)", bad[i]);
        expect_bad("0001-a", md, suffix);
    }
}

static void test_directory_name(void)
{
    expect_bad("0002-a", HDR("1", "a") "---\n", ": directory name '0002-a' "
               "does not match id and slug (expected '0001-a')");
    expect_bad("1-a", HDR("1", "a") "---\n", ": directory name '1-a' does not "
               "match id and slug (expected '0001-a')");
    expect_bad("00001-a", HDR("1", "a") "---\n", ": directory name '00001-a' "
               "does not match id and slug (expected '0001-a')");
    expect_bad("0001-b", HDR("1", "a") "---\n", ": directory name '0001-b' "
               "does not match id and slug (expected '0001-a')");
    expect_bad("0001-a-b", HDR("1", "a") "---\n", ": directory name '0001-a-b' "
               "does not match id and slug (expected '0001-a')");
    expect_bad("0001-ab", HDR("1", "a") "---\n", ": directory name '0001-ab' "
               "does not match id and slug (expected '0001-a')");
    expect_bad("0001_a", HDR("1", "a") "---\n", ": directory name '0001_a' "
               "does not match id and slug (expected '0001-a')");
    expect_bad("0123-a", HDR("123", "b") "---\n", ": directory name '0123-a' "
               "does not match id and slug (expected '0123-b')");
}

static void test_statement(void)
{
    lc_ctx *ctx;
    const lc_problem *p;

    p = load_good(&ctx, "0001-a", HDR("1", "a") "---\nLine one\n\n  Line two\n");
    if (p) {
        CHECK_STR(p->statement, "Line one\n\n  Line two\n");
        CHECK_INT(p->statement_len, 21);
        CHECK_INT(p->statement_line, 6);
    }
    lc_close(ctx);

    /* Empty statements, with and without a final newline. */
    p = load_good(&ctx, "0001-a", HDR("1", "a") "---\n");
    if (p) {
        CHECK_STR(p->statement, "");
        CHECK_INT(p->statement_len, 0);
        CHECK_INT(p->statement_line, 6);
    }
    lc_close(ctx);
    p = load_good(&ctx, "0001-a", HDR("1", "a") "---");
    if (p) {
        CHECK_STR(p->statement, "");
        CHECK_INT(p->statement_line, 6);
    }
    lc_close(ctx);

    /* Only the first '---' ends the header; the rest is kept verbatim. */
    p = load_good(&ctx, "0001-a", HDR("1", "a") "---\nid: 5\n---\n# Heading\nend");
    if (p) {
        CHECK_STR(p->statement, "id: 5\n---\n# Heading\nend");
        CHECK_INT(p->nfields, 4);
        CHECK_STR(lc_problem_field(p, "id"), "1");
    }
    lc_close(ctx);
}

static void test_missing_and_unreadable(void)
{
    const char *root;
    char path[512], want[1024];
    lc_ctx *ctx;
    int i;

    expect_bad("0001-a", NULL, ": file not found");

    /* Only a regular problem.md is read. A directory, a FIFO (opening it
     * would block) or a device (/dev/zero never ends; /dev/null stands in
     * for it) skips the pack, and the others still load. The alarm turns a
     * hang into a failure. */
    for (i = 0; i < 3; i++) {
        root = new_root();
        add_pack(root, "0001-a", NULL);
        add_pack(root, "0002-b", HDR("2", "b") "---\n");
        snprintf(path, sizeof path, "%s/0001-a/problem.md", root);
        if (i == 0)
            make_dir(path);
        else if (i == 1)
            CHECK(mkfifo(path, 0666) == 0);
        else
            CHECK(symlink("/dev/null", path) == 0);
        alarm(30);
        ctx = load_dir(root);
        alarm(0);
        CHECK_INT(lc_problem_count(ctx), 1);
        CHECK_INT(lc_warning_count(ctx), 1);
        snprintf(want, sizeof want, "%s/0001-a/problem.md: not a regular file",
                 pdir(ctx));
        CHECK_STR(lc_warning_at(ctx, 0), want);
        lc_close(ctx);
        if (i > 0)
            unlink(path); /* don't leave a FIFO for other tools to open */
    }

    /* Other read errors skip the pack too, with the reason in the warning. */
    root = new_root();
    add_pack(root, "0001-a", NULL);
    snprintf(path, sizeof path, "%s/0001-a/problem.md", root);
    CHECK(symlink("problem.md", path) == 0); /* a loop */
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 0);
    CHECK_INT(lc_warning_count(ctx), 1);
    snprintf(want, sizeof want, "%s/0001-a/problem.md: cannot open '%s/0001-a/"
             "problem.md': ", pdir(ctx), pdir(ctx));
    CHECK(lc_warning_count(ctx) == 1 &&
          strncmp(lc_warning_at(ctx, 0), want, strlen(want)) == 0);
    lc_close(ctx);

    /* A problem.md over the size limit skips the pack without being read
     * (it used to be read whole, and a big enough one made lc_load fail
     * with "out of memory"). */
    root = new_root();
    add_pack(root, "0001-a", NULL);
    add_pack(root, "0002-b", HDR("2", "b") "---\n");
    snprintf(path, sizeof path, "%s/0001-a/problem.md", root);
    make_sparse(path, (off_t)LC_MAX_PROBLEM_MD + 1);
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 1);
    CHECK_INT(lc_warning_count(ctx), 1);
    snprintf(want, sizeof want, "%s/0001-a/problem.md: file too large (over "
             "16 MiB)", pdir(ctx));
    CHECK_STR(lc_warning_at(ctx, 0), want);
    lc_close(ctx);
    unlink(path);

    /* An entry that might be a pack but can't be examined (a link loop
     * here; EOVERFLOW or EACCES elsewhere) is reported, not silently left
     * out. A dangling link is simply not a directory. */
    root = new_root();
    add_pack(root, "0002-b", HDR("2", "b") "---\n");
    snprintf(path, sizeof path, "%s/0001-loop", root);
    CHECK(symlink("0001-loop", path) == 0);
    snprintf(path, sizeof path, "%s/0003-dangling", root);
    CHECK(symlink("nowhere", path) == 0);
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 1);
    CHECK_INT(lc_warning_count(ctx), 1);
    snprintf(want, sizeof want, "%s/0001-loop: cannot access: ", pdir(ctx));
    CHECK(lc_warning_count(ctx) == 1 &&
          strncmp(lc_warning_at(ctx, 0), want, strlen(want)) == 0 &&
          strlen(lc_warning_at(ctx, 0)) > strlen(want));
    lc_close(ctx);
}

static void test_duplicates(void)
{
    const char *root;
    char want[1024];
    lc_ctx *ctx;

    /* The pack whose directory sorts first is kept. */
    root = new_root();
    add_pack(root, "0001-c", HDR("1", "c") "---\n");
    add_pack(root, "0001-a", HDR("1", "a") "---\n");
    add_pack(root, "0001-b", HDR("1", "b") "---\n");
    add_pack(root, "0002-x", HDR("2", "x") "---\n");
    add_pack(root, "0003-x", "id: 3\ntitle: T\nslug: x\ndifficulty: easy\n---\n");
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 2);
    CHECK_INT(lc_warning_count(ctx), 3);
    if (lc_warning_count(ctx) == 3) {
        snprintf(want, sizeof want, "%s/0001-b/problem.md:1: duplicate id 1 "
                 "(already used by 0001-a)", pdir(ctx));
        CHECK_STR(lc_warning_at(ctx, 0), want);
        snprintf(want, sizeof want, "%s/0001-c/problem.md:1: duplicate id 1 "
                 "(already used by 0001-a)", pdir(ctx));
        CHECK_STR(lc_warning_at(ctx, 1), want);
        snprintf(want, sizeof want, "%s/0003-x/problem.md:3: duplicate slug "
                 "'x' (already used by 0002-x)", pdir(ctx));
        CHECK_STR(lc_warning_at(ctx, 2), want);
    }
    CHECK_STR(lc_problem_find(ctx, "1") ? lc_problem_find(ctx, "1")->slug : NULL,
              "a");
    lc_close(ctx);

    /* A skipped pack does not claim its id or slug. */
    root = new_root();
    add_pack(root, "0003-a", HDR("3", "a") "---\n");
    add_pack(root, "0005-a", HDR("5", "a") "---\n"); /* slug taken: skipped */
    add_pack(root, "0005-b", HDR("5", "b") "---\n"); /* so id 5 is free */
    add_pack(root, "0006-c", "id: 6\nslug: c\ntitle: T\ndifficulty: eazy\n---\n");
    add_pack(root, "0006-d", HDR("6", "d") "---\n");
    add_pack(root, "0007-c", HDR("7", "c") "---\n");
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 4);
    CHECK_INT(lc_warning_count(ctx), 2);
    if (lc_problem_count(ctx) == 4) {
        CHECK_STR(lc_problem_at(ctx, 0)->slug, "a");
        CHECK_STR(lc_problem_at(ctx, 1)->slug, "b");
        CHECK_STR(lc_problem_at(ctx, 2)->slug, "d");
        CHECK_STR(lc_problem_at(ctx, 3)->slug, "c");
    }
    lc_close(ctx);
}

static void test_sorting(void)
{
    static const int ids[] = {2, 20, 300, 9000, 10000};
    const char *root = new_root();
    lc_ctx *ctx;
    size_t i;

    add_pack(root, "10000-e", HDR("10000", "e") "---\n");
    add_pack(root, "0300-c", HDR("300", "c") "---\n");
    add_pack(root, "0002-a", HDR("2", "a") "---\n");
    add_pack(root, "9000-d", HDR("9000", "d") "---\n");
    add_pack(root, "0020-b", HDR("20", "b") "---\n");
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), 5);
    for (i = 0; i < 5 && i < lc_problem_count(ctx); i++)
        CHECK_INT(lc_problem_at(ctx, i)->id, ids[i]);
    CHECK(lc_problem_at(ctx, 5) == NULL);
    lc_close(ctx);
}

static void expect_find(lc_ctx *ctx, const char *key, int id)
{
    const lc_problem *p = lc_problem_find(ctx, key);

    CHECK(p != NULL);
    if (!p)
        fprintf(stderr, "    no match for '%s': %s\n", key, lc_last_error(ctx));
    else if (p->id != id)
        fprintf(stderr, "    '%s' found id %d, want %d\n", key, p->id, id);
    CHECK_INT(p ? p->id : -1, id);
}

static void expect_no_find(lc_ctx *ctx, const char *key)
{
    char want[256];

    CHECK(lc_problem_find(ctx, key) == NULL);
    snprintf(want, sizeof want, "no problem matches '%s'", key);
    CHECK_STR(lc_last_error(ctx), want);
}

static void test_find(void)
{
    static const char *const misses[] = {
        "0001-3sum", "3", "0", "00", "99999999999999999999", "100000",
        "Two-Sum", "two-sum ", " 1", "1-", "-1", "1x", "0015-", "15-3sum-x",
        "abc", "0002-2-keys-keyboard", "10000-", "1-two-sum-"
    };
    const char *root = new_root();
    lc_ctx *ctx;
    size_t i;

    add_pack(root, "0001-two-sum", HDR("1", "two-sum") "---\n");
    add_pack(root, "0015-3sum", HDR("15", "3sum") "---\n");
    add_pack(root, "0650-2-keys-keyboard", HDR("650", "2-keys-keyboard") "---\n");
    add_pack(root, "0002-keys-keyboard", HDR("2", "keys-keyboard") "---\n");
    add_pack(root, "10000-big", HDR("10000", "big") "---\n");

    /* Before lc_load. */
    ctx = open_dir(root);
    CHECK(lc_problem_find(ctx, "1") == NULL);
    CHECK_STR(lc_last_error(ctx), "problems are not loaded");
    CHECK_INT(lc_problem_count(ctx), 0);
    CHECK(lc_problem_at(ctx, 0) == NULL);
    CHECK(lc_problems_dir(ctx) == NULL);
    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_INT(lc_problem_count(ctx), 5);

    /* Slugs, including ones that look like numbers or "N-slug". */
    expect_find(ctx, "two-sum", 1);
    expect_find(ctx, "3sum", 15);
    expect_find(ctx, "2-keys-keyboard", 650); /* not id 2 + "keys-keyboard" */
    expect_find(ctx, "keys-keyboard", 2);
    expect_find(ctx, "big", 10000);
    /* Numbers, with or without leading zeros. */
    expect_find(ctx, "1", 1);
    expect_find(ctx, "0001", 1);
    expect_find(ctx, "000000000000000000001", 1);
    expect_find(ctx, "15", 15);
    expect_find(ctx, "0015", 15);
    expect_find(ctx, "2", 2);
    expect_find(ctx, "650", 650);
    expect_find(ctx, "10000", 10000);
    expect_find(ctx, "010000", 10000);
    /* Directory names: id and slug must both match. */
    expect_find(ctx, "0001-two-sum", 1);
    expect_find(ctx, "1-two-sum", 1);
    expect_find(ctx, "0015-3sum", 15);
    expect_find(ctx, "0650-2-keys-keyboard", 650);
    expect_find(ctx, "650-2-keys-keyboard", 650);
    expect_find(ctx, "0002-keys-keyboard", 2);
    expect_find(ctx, "10000-big", 10000);

    for (i = 0; i < sizeof misses / sizeof misses[0]; i++)
        expect_no_find(ctx, misses[i]);
    CHECK(lc_problem_find(ctx, "") == NULL);
    CHECK_STR(lc_last_error(ctx), "no problem given");
    CHECK(lc_problem_find(ctx, NULL) == NULL);
    CHECK_STR(lc_last_error(ctx), "no problem given");
    lc_close(ctx);

    /* An empty directory matches nothing. */
    ctx = load_dir(new_root());
    expect_no_find(ctx, "1");
    expect_no_find(ctx, "two-sum");
    lc_close(ctx);
}

static void test_field_and_tag(void)
{
    lc_ctx *ctx = load_dir(FIXTURES);
    const lc_problem *p = lc_problem_find(ctx, "two-sum");

    CHECK(p != NULL);
    if (p) {
        CHECK_STR(lc_problem_field(p, "id"), "1");
        CHECK_STR(lc_problem_field(p, "slug"), "two-sum");
        CHECK_STR(lc_problem_field(p, "tags"), "array, hash-table");
        CHECK_STR(lc_problem_field(p, "compare"), "unordered 0");
        CHECK(lc_problem_field(p, "hidden") == NULL);
        CHECK(lc_problem_field(p, "Compare") == NULL);
        CHECK(lc_problem_field(p, "") == NULL);
        CHECK(lc_problem_field(p, NULL) == NULL);
        CHECK(lc_problem_has_tag(p, "array") == 1);
        CHECK(lc_problem_has_tag(p, "hash-table") == 1);
        CHECK(lc_problem_has_tag(p, "hash") == 0);
        CHECK(lc_problem_has_tag(p, "Array") == 0);
        CHECK(lc_problem_has_tag(p, "") == 0);
        CHECK(lc_problem_has_tag(p, NULL) == 0);
    }
    CHECK(lc_problem_field(NULL, "id") == NULL);
    CHECK(lc_problem_has_tag(NULL, "array") == 0);
    lc_close(ctx);
}

/* Many packs: the duplicate checks and lookups must stay fast. */
static void test_many_packs(void)
{
    enum { N = 3000 };
    const char *root = new_root();
    char name[64], md[256], key[64];
    lc_ctx *ctx;
    size_t i;
    int ok = 1;

    for (i = 1; i <= N; i++) {
        snprintf(name, sizeof name, "%04d-p%d", (int)i, (int)i);
        snprintf(md, sizeof md, "id: %d\nslug: p%d\ntitle: Problem %d\n"
                 "difficulty: medium\ntags: t%d\n---\nStatement %d.\n",
                 (int)i, (int)i, (int)i, (int)(i % 7), (int)i);
        add_pack(root, name, md);
    }
    /* Two duplicates at the end. */
    add_pack(root, "3001-p1", HDR("3001", "p1") "---\n");
    add_pack(root, "3000-q", HDR("3000", "q") "---\n");
    ctx = load_dir(root);
    CHECK_INT(lc_problem_count(ctx), N);
    CHECK_INT(lc_warning_count(ctx), 2);
    for (i = 0; i < lc_problem_count(ctx); i++)
        ok &= lc_problem_at(ctx, i)->id == (int)i + 1;
    CHECK(ok);
    for (i = 1; i <= N; i += 37) {
        snprintf(key, sizeof key, "p%d", (int)i);
        expect_find(ctx, key, (int)i);
        snprintf(key, sizeof key, "%d", (int)i);
        expect_find(ctx, key, (int)i);
    }
    expect_no_find(ctx, "3001");
    lc_close(ctx);
}

int main(void)
{
    /* Start from a clean scratch directory. */
    if (system("rm -rf " TMP) != 0)
        return 1;
    make_dir(TMP);
    unsetenv("LC_PROBLEMS");
    RUN(test_search_order);
    RUN(test_empty_and_ignored);
    RUN(test_double_load);
    RUN(test_fixture_tree);
    RUN(test_bad_tree);
    RUN(test_header_syntax);
    RUN(test_bom_crlf);
    RUN(test_nul_byte);
    RUN(test_required_fields);
    RUN(test_id);
    RUN(test_slug_title_difficulty);
    RUN(test_tags);
    RUN(test_signature_and_hidden);
    RUN(test_typed_fields);
    RUN(test_limits);
    RUN(test_directory_name);
    RUN(test_statement);
    RUN(test_missing_and_unreadable);
    RUN(test_duplicates);
    RUN(test_sorting);
    RUN(test_find);
    RUN(test_field_and_tag);
    RUN(test_many_packs);
    return test_report();
}
