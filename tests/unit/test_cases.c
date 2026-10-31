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

#define TMP "tests/tmp/cases"
#define FIXTURES "tests/fixtures/problems"

static void make_dir(const char *path)
{
    lc_ctx *ctx = lc_open(NULL);
    CHECK_INT(lc_mkdir_p(ctx, path, 0777), LC_OK);
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

static void write_file(const char *path, const char *data)
{
    FILE *f = fopen(path, "wb");

    CHECK(f != NULL);
    if (!f)
        return;
    CHECK(fwrite(data, 1, strlen(data), f) == strlen(data));
    CHECK(fclose(f) == 0);
}

/* Parses `text` as "t.txt"; the result must be released with lc_cases_free. */
static lc_cases *parse_ok(lc_ctx *ctx, const char *text)
{
    lc_cases *cs = NULL;
    int rc = lc_parse_cases(ctx, text, strlen(text), "t.txt", &cs);

    CHECK_INT(rc, LC_OK);
    if (rc != LC_OK)
        fprintf(stderr, "    error: %s\n", lc_last_error(ctx));
    CHECK(cs != NULL);
    return cs;
}

static void expect_error(const char *text, const char *msg)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = NULL;

    CHECK_INT(lc_parse_cases(ctx, text, strlen(text), "t.txt", &cs), LC_ERR_PARSE);
    CHECK(cs == NULL);
    CHECK_STR(lc_last_error(ctx), msg);
    lc_cases_free(cs);
    lc_close(ctx);
}

/* Checks one case. `args` is NULL-terminated. */
static void check_case(const lc_cases *cs, size_t i, const char *const *args,
                       const char *expected, const char *explanation,
                       int example, int line)
{
    const lc_case *c;
    size_t n = 0, k;

    CHECK(cs != NULL && i < cs->count);
    if (!cs || i >= cs->count)
        return;
    c = &cs->items[i];
    while (args[n])
        n++;
    CHECK_INT(c->nargs, n);
    for (k = 0; k < n && k < c->nargs; k++)
        CHECK_STR(c->args[k], args[k]);
    CHECK_STR(c->expected, expected);
    CHECK_STR(c->explanation, explanation);
    CHECK_INT(c->example, example);
    CHECK_INT(c->line, line);
}

#define ARGS(...) ((const char *const[]){__VA_ARGS__, NULL})

static const char spec_example[] =
    "# Comments start with '#'.\n"
    "@example 4 + 9 = 13, so the answer is the indices of 4 and 9.\n"
    "[4,1,9]\n"
    "13\n"
    "=> [0,2]\n"
    "\n"
    "@example\n"
    "[5,5]\n"
    "10\n"
    "=> [0,1]\n"
    "\n"
    "[1,2,3,4]\n"
    "7\n"
    "=> [2,3]\n";

static void check_spec_example(const lc_cases *cs)
{
    CHECK(cs != NULL);
    if (!cs)
        return;
    CHECK_INT(cs->count, 3);
    check_case(cs, 0, ARGS("[4,1,9]", "13"), "[0,2]",
               "4 + 9 = 13, so the answer is the indices of 4 and 9.", 1, 3);
    check_case(cs, 1, ARGS("[5,5]", "10"), "[0,1]", NULL, 1, 8);
    check_case(cs, 2, ARGS("[1,2,3,4]", "7"), "[2,3]", NULL, 0, 12);
}

static void test_spec_example(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = parse_ok(ctx, spec_example);

    check_spec_example(cs);
    lc_cases_free(cs);
    lc_close(ctx);
}

static void test_crlf_and_bom(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_str s = LC_STR_INIT;
    lc_cases *cs;
    size_t i;

    /* The same file with CRLF line endings and a byte order mark. */
    lc_str_puts(&s, "\xEF\xBB\xBF");
    for (i = 0; spec_example[i]; i++) {
        if (spec_example[i] == '\n')
            lc_str_putc(&s, '\r');
        lc_str_putc(&s, spec_example[i]);
    }
    cs = parse_ok(ctx, s.data);
    check_spec_example(cs);
    lc_cases_free(cs);

    /* Only the BOM, or a BOM and nothing else of use. */
    cs = parse_ok(ctx, "\xEF\xBB\xBF");
    CHECK(cs && cs->count == 0);
    lc_cases_free(cs);
    cs = parse_ok(ctx, "\xEF\xBB\xBF# c\r\n\r\n");
    CHECK(cs && cs->count == 0);
    lc_cases_free(cs);

    /* No final newline; a lone '\r' inside a line is kept. */
    cs = parse_ok(ctx, "a\rb\r\n=> 1");
    CHECK(cs && cs->count == 1);
    check_case(cs, 0, ARGS("a\rb"), "1", NULL, 0, 1);
    lc_cases_free(cs);

    /* A BOM is only skipped at the start. */
    cs = parse_ok(ctx, "1\n=> 2\n\xEF\xBB\xBF" "3\n=> 4\n");
    check_case(cs, 1, ARGS("\xEF\xBB\xBF" "3"), "4", NULL, 0, 3);
    lc_cases_free(cs);
    lc_str_free(&s);
    lc_close(ctx);
}

static void test_trimming(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs;

    cs = parse_ok(ctx, "  [1, 2]  \t\n\t\"a b\"\n=>   [3,  4]  \n"
                       "x\n=>y\n"
                       "z\n\t => w\n");
    CHECK(cs && cs->count == 3);
    /* Spaces and tabs around a line go; spaces inside it stay. */
    check_case(cs, 0, ARGS("[1, 2]", "\"a b\""), "[3,  4]", NULL, 0, 1);
    /* "=>" needs no space after it, and may be indented. */
    check_case(cs, 1, ARGS("x"), "y", NULL, 0, 4);
    check_case(cs, 2, ARGS("z"), "w", NULL, 0, 6);
    lc_cases_free(cs);
    lc_close(ctx);
}

static void test_comments_and_blanks(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs;

    cs = parse_ok(ctx,
                  "# leading comment\n"
                  "\n"
                  " \t \n"
                  "#\n"
                  "@example\n"
                  "\n"
                  "# between the marker and its case\n"
                  "[1]\n"
                  "# inside a case\n"
                  "  # indented comment\n"
                  "\"a#b\"\n"
                  "# just before =>\n"
                  "=> 2 # not a comment\n"
                  "\n\n\n"
                  "3\n"
                  "=> 4\n"
                  "# trailing comment");
    CHECK(cs && cs->count == 2);
    check_case(cs, 0, ARGS("[1]", "\"a#b\""), "2 # not a comment", NULL, 1, 8);
    check_case(cs, 1, ARGS("3"), "4", NULL, 0, 17);
    lc_cases_free(cs);
    lc_close(ctx);
}

static void test_explanations(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs;

    cs = parse_ok(ctx,
                  "@example\n1\n=> 1\n"
                  "@example   \n2\n=> 2\n"
                  "@example   two  spaces  \n3\n=> 3\n"
                  "@example\ttabbed\n4\n=> 4\n"
                  "  @example indented, see #3\n5\n=> 5\n"
                  "6\n=> 6\n");
    CHECK(cs && cs->count == 6);
    check_case(cs, 0, ARGS("1"), "1", NULL, 1, 2);
    check_case(cs, 1, ARGS("2"), "2", NULL, 1, 5);
    check_case(cs, 2, ARGS("3"), "3", "two  spaces", 1, 8);
    check_case(cs, 3, ARGS("4"), "4", "tabbed", 1, 11);
    check_case(cs, 4, ARGS("5"), "5", "indented, see #3", 1, 14);
    check_case(cs, 5, ARGS("6"), "6", NULL, 0, 16);
    lc_cases_free(cs);
    lc_close(ctx);
}

static void test_empty(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs;

    cs = parse_ok(ctx, "");
    CHECK(cs && cs->count == 0 && cs->items != NULL);
    lc_cases_free(cs);
    cs = parse_ok(ctx, "# only comments\n\n   \n");
    CHECK(cs && cs->count == 0);
    lc_cases_free(cs);
    cs = NULL;
    CHECK_INT(lc_parse_cases(ctx, NULL, 0, "t.txt", &cs), LC_OK);
    CHECK(cs && cs->count == 0);
    lc_cases_free(cs);
    lc_close(ctx);
}

static void test_nul_byte(void)
{
    static const char text[] = "1\n=> 2\n\n[1,\0002]\n=> 3\n";
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = NULL;

    CHECK_INT(lc_parse_cases(ctx, text, sizeof text - 1, "t.txt", &cs),
              LC_ERR_PARSE);
    CHECK(cs == NULL);
    CHECK_STR(lc_last_error(ctx), "t.txt:4: line contains a NUL byte");
    lc_close(ctx);
}

static void test_errors(void)
{
    static char long_directive[128];

    expect_error("1\n\n=> 2\n",
                 "t.txt:2: blank line inside a case (missing '=>'?)");
    expect_error("1\n2\n  \t\n=> 3\n",
                 "t.txt:3: blank line inside a case (missing '=>'?)");
    expect_error("1\r\n\r\n=> 2\r\n",
                 "t.txt:2: blank line inside a case (missing '=>'?)");
    expect_error("\xEF\xBB\xBF" "1\n\n=> 2\n",
                 "t.txt:2: blank line inside a case (missing '=>'?)");

    expect_error("=> 2\n", "t.txt:1: '=>' with no input lines before it");
    expect_error("# c\n\n=> 2\n", "t.txt:3: '=>' with no input lines before it");
    expect_error("1\n=> 2\n=> 3\n", "t.txt:3: '=>' with no input lines before it");
    expect_error("1\n=> 2\n  => 3\n",
                 "t.txt:3: '=>' with no input lines before it");
    expect_error("@example\n=> 3\n",
                 "t.txt:2: '=>' with no input lines before it");

    expect_error("1\n=>\n", "t.txt:2: nothing after '=>'");
    expect_error("1\n=>  \t \n", "t.txt:2: nothing after '=>'");

    expect_error("@example\n", "t.txt:1: @example is not followed by a case");
    expect_error("1\n=> 2\n@example why\n\n# c\n",
                 "t.txt:3: @example is not followed by a case");
    expect_error("@example\n@example\n1\n=> 2\n",
                 "t.txt:2: two @example lines in a row");
    expect_error("@example a\n\n# c\n@example b\n1\n=> 2\n",
                 "t.txt:4: two @example lines in a row");
    expect_error("1\n@example\n=> 2\n",
                 "t.txt:2: @example inside a case (missing '=>'?)");

    expect_error("@examples\n", "t.txt:1: unknown directive '@examples'");
    expect_error("1\n=> 2\n@ex 1\n", "t.txt:3: unknown directive '@ex'");
    expect_error("@\n", "t.txt:1: unknown directive '@'");
    expect_error("@Example\n1\n=> 2\n", "t.txt:1: unknown directive '@Example'");
    expect_error("@example: x\n1\n=> 2\n",
                 "t.txt:1: unknown directive '@example:'");
    memset(long_directive, 'x', 100);
    long_directive[0] = '@';
    long_directive[100] = '\n';
    expect_error(long_directive, "t.txt:1: unknown directive '@xxxxxxxxxxxxxxxxx"
                 "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx...'");

    /* ... cut at a UTF-8 character boundary. */
    memset(long_directive, 'y', 59);
    long_directive[0] = '@';
    strcpy(long_directive + 59, "\xC3\xA9zz\n");
    expect_error(long_directive, "t.txt:1: unknown directive '@yyyyyyyyyyyyyyyyy"
                 "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy...'");

    expect_error("1\n2\n", "t.txt:1: case has no '=>' line");
    expect_error("1\n=> 2\n\n3\n# c\n", "t.txt:4: case has no '=>' line");
    expect_error("1", "t.txt:1: case has no '=>' line");
}

/* @gen marks a case gen.c makes; lc_case keeps its arguments and where
 * its lines are, for lc check --regen. */
static void test_gen_directive(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = parse_ok(ctx, "# c\n@gen 7\n[1,2]\n# inside\n3\n=> 4\n"
                                 "\n@gen  12   small\n5\n=> 6\n"
                                 "@example\n7\n=> 8\n");

    if (cs && cs->count == 3) {
        CHECK_STR(cs->items[0].gen, "7");
        CHECK_INT(cs->items[0].line, 3);
        CHECK_INT(cs->items[0].last_line, 5);
        CHECK_INT(cs->items[0].expected_line, 6);
        CHECK_INT(cs->items[0].example, 0);
        CHECK_STR(cs->items[1].gen, "12 small");
        CHECK_INT(cs->items[1].line, 9);
        CHECK_INT(cs->items[1].last_line, 9);
        CHECK_INT(cs->items[1].expected_line, 10);
        CHECK(cs->items[2].gen == NULL);
        CHECK_INT(cs->items[2].example, 1);
    } else {
        CHECK(0);
    }
    lc_cases_free(cs);
    cs = parse_ok(ctx, "@gen 18446744073709551615 small\n1\n=> 2\n");
    if (cs && cs->count == 1)
        CHECK_STR(cs->items[0].gen, "18446744073709551615 small");
    lc_cases_free(cs);
    lc_close(ctx);

    expect_error("@gen\n1\n=> 2\n", "t.txt:1: expected '@gen SEED' or "
                 "'@gen SEED small', where SEED is a number");
    expect_error("@gen x\n1\n=> 2\n", "t.txt:1: expected '@gen SEED' or "
                 "'@gen SEED small', where SEED is a number");
    expect_error("@gen 1 big\n1\n=> 2\n", "t.txt:1: expected '@gen SEED' or "
                 "'@gen SEED small', where SEED is a number");
    expect_error("@gen 07\n1\n=> 2\n", "t.txt:1: expected '@gen SEED' or "
                 "'@gen SEED small', where SEED is a number");
    expect_error("@gen 18446744073709551616\n1\n=> 2\n", "t.txt:1: expected "
                 "'@gen SEED' or '@gen SEED small', where SEED is a number");
    expect_error("@gen 1\n", "t.txt:1: @gen is not followed by a case (run "
                 "'lc check --regen' to make it)");
    expect_error("@gen 1\n\n1\n=> 2\n", "t.txt:1: @gen is not followed by a "
                 "case (run 'lc check --regen' to make it)");
    expect_error("@gen 1\n@gen 2\n1\n=> 2\n", "t.txt:2: two @gen lines in a "
                 "row");
    expect_error("@example\n@gen 2\n1\n=> 2\n", "t.txt:2: a case can't be "
                 "both @example and @gen");
    expect_error("@gen 2\n@example\n1\n=> 2\n", "t.txt:2: a case can't be "
                 "both @example and @gen");
    expect_error("1\n@gen 2\n=> 2\n", "t.txt:2: @gen inside a case (missing "
                 "'=>'?)");
    expect_error("@gen 2\n=> 2\n", "t.txt:2: '=>' with no input lines before "
                 "it");
}

/* --regen reads tests.txt leniently: cases may lack their "=>", and a
 * bare @gen line is a case still to be made. */
static void test_regen_mode(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = NULL;
    const char *text = "@gen 1 small\n\n@gen 2\n@gen 3\n=> 9\n# hand\n"
                       "[3,1]\n\n[4]\n=>\n@example\n1\n=> 2\n@gen 4\n";
    int rc = lc_parse_regen_cases(ctx, text, strlen(text), "t.txt", &cs);

    CHECK_INT(rc, LC_OK);
    if (rc == LC_OK && cs->count == 7) {
        /* "@gen 1 small", then a blank line: no inputs yet. */
        CHECK_STR(cs->items[0].gen, "1 small");
        CHECK_INT(cs->items[0].nargs, 0);
        CHECK_INT(cs->items[0].line, 1);
        CHECK_INT(cs->items[0].last_line, 0);
        CHECK_INT(cs->items[0].expected_line, 0);
        /* "@gen 2", ended by the next directive. */
        CHECK_STR(cs->items[1].gen, "2");
        CHECK_INT(cs->items[1].line, 3);
        /* "@gen 3", then "=> 9" with no inputs. */
        CHECK_STR(cs->items[2].gen, "3");
        CHECK_INT(cs->items[2].nargs, 0);
        CHECK_STR(cs->items[2].expected, "9");
        CHECK_INT(cs->items[2].expected_line, 5);
        /* A case without "=>", ended by a blank line. */
        CHECK(cs->items[3].gen == NULL);
        CHECK_INT(cs->items[3].nargs, 1);
        CHECK(cs->items[3].expected == NULL);
        CHECK_INT(cs->items[3].last_line, 7);
        /* "=>" with nothing after it: to be filled in. */
        CHECK(cs->items[4].expected == NULL);
        CHECK_INT(cs->items[4].expected_line, 10);
        CHECK_INT(cs->items[5].example, 1);
        /* "@gen 4" at the end of the file. */
        CHECK_STR(cs->items[6].gen, "4");
        CHECK_INT(cs->items[6].line, 14);
    } else {
        CHECK(0);
    }
    lc_cases_free(cs);
    cs = NULL;
    /* Still errors: an @example with no case, a bad directive. */
    CHECK_INT(lc_parse_regen_cases(ctx, "@example\n", 9, "t.txt", &cs),
              LC_ERR_PARSE);
    CHECK_STR(lc_last_error(ctx), "t.txt:1: @example is not followed by a "
              "case");
    CHECK_INT(lc_parse_regen_cases(ctx, "@gen x\n", 7, "t.txt", &cs),
              LC_ERR_PARSE);
    /* Your cases may not use @gen. */
    CHECK_INT(lc_parse_user_cases(ctx, "@gen 1\n1\n", 9, "c.txt", &cs),
              LC_ERR_PARSE);
    CHECK_STR(lc_last_error(ctx), "c.txt:1: @gen is only for a problem's "
              "tests.txt");
    lc_close(ctx);
}

static void test_filename_and_misuse(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = NULL;

    CHECK_INT(lc_parse_cases(ctx, "=> 1", 4, "dir/x/tests.txt", &cs), LC_ERR_PARSE);
    CHECK_STR(lc_last_error(ctx),
              "dir/x/tests.txt:1: '=>' with no input lines before it");
    CHECK_INT(lc_parse_cases(ctx, "=> 1", 4, NULL, &cs), LC_ERR_PARSE);
    CHECK_STR(lc_last_error(ctx),
              "tests.txt:1: '=>' with no input lines before it");
    CHECK(cs == NULL);

    /* `len` is honoured: text after it is not read. */
    CHECK_INT(lc_parse_cases(ctx, "1\n=> 2\ngarbage", 7, "t", &cs), LC_OK);
    CHECK(cs && cs->count == 1);
    lc_cases_free(cs);

    CHECK_INT(lc_parse_cases(ctx, "1\n=> 2\n", 7, "t", NULL), LC_ERR_INVALID);
    cs = (lc_cases *)&cs; /* must be reset */
    CHECK_INT(lc_parse_cases(ctx, NULL, 3, "t", &cs), LC_ERR_INVALID);
    CHECK(cs == NULL);
    lc_cases_free(NULL);
    lc_close(ctx);
}

static void test_many_cases(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_str s = LC_STR_INIT;
    lc_cases *cs;
    int i, ok = 1;

    for (i = 0; i < 10000; i++)
        lc_str_printf(&s, "%s[%d,%d]\n%d\n=> %d\n\n", i % 3 ? "" : "@example\n",
                      i, i + 1, i, 2 * i + 1);
    cs = parse_ok(ctx, s.data);
    CHECK(cs && cs->count == 10000);
    for (i = 0; cs && i < (int)cs->count; i++) {
        const lc_case *c = &cs->items[i];
        ok &= c->nargs == 2 && c->example == (i % 3 == 0) &&
              atoi(c->expected) == 2 * i + 1;
    }
    CHECK(ok);
    lc_cases_free(cs);
    lc_str_free(&s);
    lc_close(ctx);
}

static void test_load_cases(void)
{
    static const char *const special[] = {"dir", "fifo", "dev"};
    lc_options opt;
    lc_ctx *ctx;
    const lc_problem *p;
    lc_problem fake;
    lc_cases *cs = NULL;
    char dir[256], want[512];
    int i;

    lc_options_init(&opt);
    opt.problems_dir = FIXTURES;
    ctx = lc_open(&opt);
    CHECK_INT(lc_load(ctx), LC_OK);

    p = lc_problem_find(ctx, "two-sum");
    CHECK(p != NULL);
    if (p) {
        CHECK_INT(lc_load_cases(ctx, p, &cs), LC_OK);
        CHECK(cs && cs->count == 5);
        check_case(cs, 0, ARGS("[6,3,1,8]", "11"), "[1,3]",
                   "3 + 8 = 11, and those values sit at positions 1 and 3.", 1, 3);
        check_case(cs, 1, ARGS("[-4,-4]", "-8"), "[0,1]", NULL, 1, 8);
        check_case(cs, 2, ARGS("[10,20,30,40,50]", "90"), "[3,4]", NULL, 0, 12);
        check_case(cs, 4, ARGS("[-1000000,999999,1]", "-999999"), "[0,2]", NULL,
                   0, 21);
        lc_cases_free(cs);
    }

    /* CRLF line endings and a byte order mark. */
    p = lc_problem_find(ctx, "650");
    CHECK(p != NULL);
    if (p) {
        CHECK_INT(lc_load_cases(ctx, p, &cs), LC_OK);
        CHECK(cs && cs->count == 4);
        check_case(cs, 0, ARGS("3"), "3", "Copy All, then Paste twice: A, AA, AAA.",
                   1, 2);
        check_case(cs, 1, ARGS("1"), "0", NULL, 1, 6);
        check_case(cs, 3, ARGS("7"), "7", NULL, 0, 12);
        lc_cases_free(cs);
    }

    /* No tests.txt: an empty list, and the last error is left alone. */
    p = lc_problem_find(ctx, "min-stack");
    CHECK(p != NULL);
    if (p) {
        CHECK_STR(lc_last_error(ctx), "");
        CHECK_INT(lc_load_cases(ctx, p, &cs), LC_OK);
        CHECK(cs && cs->count == 0);
        CHECK_STR(lc_last_error(ctx), "");
        lc_cases_free(cs);
    }

    /* Errors name the full path of tests.txt. */
    make_dir(TMP "/bad");
    write_file(TMP "/bad/tests.txt", "# c\n1\n\n=> 2\n");
    memset(&fake, 0, sizeof fake);
    fake.dir = TMP "/bad";
    cs = (lc_cases *)&cs;
    CHECK_INT(lc_load_cases(ctx, &fake, &cs), LC_ERR_PARSE);
    CHECK(cs == NULL);
    CHECK_STR(lc_last_error(ctx), TMP "/bad/tests.txt:3: blank line inside a "
              "case (missing '=>'?)");

    /* A pack directory that doesn't exist also has no tests.txt. */
    fake.dir = TMP "/missing";
    CHECK_INT(lc_load_cases(ctx, &fake, &cs), LC_OK);
    CHECK(cs && cs->count == 0);
    lc_cases_free(cs);

    /* Only a regular tests.txt is read: not a directory, a FIFO (opening it
     * would block) or a device (/dev/null stands in for /dev/zero, which
     * never ends). The alarm turns a hang into a failure. */
    make_dir(TMP "/dir/tests.txt");
    make_dir(TMP "/fifo");
    CHECK(mkfifo(TMP "/fifo/tests.txt", 0666) == 0);
    make_dir(TMP "/dev");
    CHECK(symlink("/dev/null", TMP "/dev/tests.txt") == 0);
    for (i = 0; i < 3; i++) {
        snprintf(dir, sizeof dir, TMP "/%s", special[i]);
        snprintf(want, sizeof want, "cannot read '%s/tests.txt': not a regular "
                 "file", dir);
        fake.dir = dir;
        cs = (lc_cases *)&cs;
        alarm(30);
        CHECK_INT(lc_load_cases(ctx, &fake, &cs), LC_ERR_IO);
        alarm(0);
        CHECK(cs == NULL);
        CHECK_STR(lc_last_error(ctx), want);
    }
    unlink(TMP "/fifo/tests.txt"); /* don't leave a FIFO for other tools */
    unlink(TMP "/dev/tests.txt");

    /* Other read errors name the file. */
    make_dir(TMP "/loop");
    CHECK(symlink("tests.txt", TMP "/loop/tests.txt") == 0);
    fake.dir = TMP "/loop";
    CHECK_INT(lc_load_cases(ctx, &fake, &cs), LC_ERR_IO);
    CHECK(cs == NULL);
    CHECK(strncmp(lc_last_error(ctx), "cannot open '" TMP "/loop/tests.txt': ",
                  strlen("cannot open '" TMP "/loop/tests.txt': ")) == 0);

    /* So does a file over the size limit, which is not read at all (it
     * used to be, and a big enough one failed with "out of memory"). */
    make_dir(TMP "/huge");
    make_sparse(TMP "/huge/tests.txt", (off_t)LC_MAX_TESTS_TXT + 1);
    fake.dir = TMP "/huge";
    cs = (lc_cases *)&cs;
    CHECK_INT(lc_load_cases(ctx, &fake, &cs), LC_ERR_IO);
    CHECK(cs == NULL);
    CHECK_STR(lc_last_error(ctx), "cannot read '" TMP "/huge/tests.txt': file "
              "too large (over 256 MiB)");
    unlink(TMP "/huge/tests.txt");

    /* Misuse. */
    CHECK_INT(lc_load_cases(ctx, NULL, &cs), LC_ERR_INVALID);
    fake.dir = NULL;
    CHECK_INT(lc_load_cases(ctx, &fake, &cs), LC_ERR_INVALID);
    CHECK_INT(lc_load_cases(ctx, p, NULL), LC_ERR_INVALID);
    lc_close(ctx);
}

static void test_no_inputs(void)
{
    lc_ctx *ctx = lc_open(NULL);
    static const char *const none[] = {NULL};
    lc_cases *cs;

    cs = parse_ok(ctx, "()\n=> \"Hello World\"\n\n@example Text.\n  ()  \n"
                  "# c\n=> \"x\"\n");
    CHECK(cs && cs->count == 2);
    check_case(cs, 0, none, "\"Hello World\"", NULL, 0, 1);
    check_case(cs, 1, none, "\"x\"", "Text.", 1, 5);
    if (cs && cs->count == 2)
        CHECK(cs->items[0].args != NULL);
    lc_cases_free(cs);
    /* Only "()" exactly: anything else is an ordinary input line. */
    cs = parse_ok(ctx, "( )\n=> 1\n");
    check_case(cs, 0, ARGS("( )"), "1", NULL, 0, 1);
    lc_cases_free(cs);
    lc_close(ctx);

    expect_error("()\n()\n=> 1\n",
                 "t.txt:2: '()' must be the only input line of its case");
    expect_error("1\n()\n=> 1\n",
                 "t.txt:2: '()' must be the only input line of its case");
    expect_error("()\n1\n=> 2\n", "t.txt:2: input line after '()' (a case "
                 "with no inputs has no other input lines)");
    expect_error("()\n\n=> 1\n",
                 "t.txt:2: blank line inside a case (missing '=>'?)");
    expect_error("()\n@example\n=> 1\n",
                 "t.txt:2: @example inside a case (missing '=>'?)");
    expect_error("x\n()\n", "t.txt:2: '()' must be the only input line of "
                 "its case");
    expect_error("# c\n()\n", "t.txt:2: case has no '=>' line");
    expect_error("()\n=>\n", "t.txt:2: nothing after '=>'");
}

/* A problem with signature `sig` (or none) and `hidden` inputs, for
 * lc_case_values. Its strings live in `a`. */
static lc_problem make_problem(lc_arena *a, const char *sig,
                               const char *hidden)
{
    lc_problem p;
    lc_str err = LC_STR_INIT;
    lc_param *hp = NULL;
    size_t nh = 0;

    memset(&p, 0, sizeof p);
    p.dir = "d";
    if (sig) {
        lc_sig *s = lc_arena_zalloc(a, sizeof *s);
        CHECK_INT(lc_parse_sig(a, sig, strlen(sig), "signature", s, &err),
                  LC_OK);
        p.sig = s;
    }
    if (hidden)
        CHECK_INT(lc_parse_params(a, hidden, strlen(hidden), "hidden", &hp,
                                  &nh, &err), LC_OK);
    p.hidden = hp;
    p.nhidden = nh;
    lc_str_free(&err);
    return p;
}

/* Parses tests.txt `text` (one case) and its values for `p`; msg is the
 * expected error, or NULL. Returns the values in *args and *expected. */
static int case_values(const lc_problem *p, const char *text, const char *msg,
                       lc_pool *pool, const lc_value **args,
                       const lc_value **expected)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = parse_ok(ctx, text);
    int rc = LC_ERR_INVALID;

    if (cs && cs->count == 1) {
        /* Without a file of its own, a case's errors name the pack's
         * tests.txt (test_case_file checks the other way). */
        lc_case c = cs->items[0];
        c.file = NULL;
        rc = lc_case_values(ctx, p, &c, pool, args, expected);
        if (msg) {
            CHECK_INT(rc, LC_ERR_PARSE);
            CHECK_STR(lc_last_error(ctx), msg);
            CHECK(*args == NULL && *expected == NULL);
        } else {
            CHECK_INT(rc, LC_OK);
            if (rc != LC_OK)
                fprintf(stderr, "    error: %s\n", lc_last_error(ctx));
        }
    }
    lc_cases_free(cs);
    lc_close(ctx);
    return rc;
}

static void test_case_values(void)
{
    lc_arena a = {0};
    lc_pool *pool = lc_pool_new();
    const lc_value *args = NULL, *exp = NULL;
    lc_problem p;

    p = make_problem(&a, "int[] twoSum(int[] nums, int target)", NULL);
    if (case_values(&p, "[4,1,9]\n13\n=> [0,2]\n", NULL, pool, &args,
                    &exp) == LC_OK) {
        CHECK_INT(args[0].kind, LC_V_ARRAY);
        CHECK_INT(args[0].len, 3);
        CHECK_INT(args[1].kind, LC_V_INT);
        CHECK_INT(args[1].u.i, 13);
        CHECK_INT(exp->kind, LC_V_ARRAY);
        CHECK_INT(exp->len, 2);
    }
    case_values(&p, "[1]\n=> [0]\n", "d/tests.txt:1: case has 1 input, "
                "expected 2 (nums, target)", pool, &args, &exp);
    case_values(&p, "# x\n[1]\n2\n3\n=> [0]\n", "d/tests.txt:2: case has 3 "
                "inputs, expected 2 (nums, target)", pool, &args, &exp);
    case_values(&p, "[1]\n\"x\"\n=> [0]\n", "d/tests.txt:1: target: column "
                "1: expected int, got a string", pool, &args, &exp);
    case_values(&p, "[1,]\n2\n=> [0]\n", "d/tests.txt:1: nums: column 4: "
                "expected a value after ','", pool, &args, &exp);
    case_values(&p, "[1]\n2\n=> 0\n", "d/tests.txt:1: expected output: "
                "column 1: expected int[], got 0", pool, &args, &exp);

    p = make_problem(&a, "bool hasCycle(ListNode head)", "int pos");
    case_values(&p, "[3,2,0,-4]\n1\n=> true\n", NULL, pool, &args, &exp);
    case_values(&p, "[1]\n=> true\n", "d/tests.txt:1: case has 1 input, "
                "expected 2 (head, pos)", pool, &args, &exp);
    case_values(&p, "[1]\n1.5\n=> true\n", "d/tests.txt:1: pos: column 1: "
                "expected int, got 1.5", pool, &args, &exp);

    p = make_problem(&a, "string helloWorld()", NULL);
    case_values(&p, "()\n=> \"Hello World\"\n", NULL, pool, &args, &exp);
    case_values(&p, "1\n=> \"x\"\n", "d/tests.txt:1: case has 1 input, "
                "expected 0", pool, &args, &exp);

    /* A void result without check: is parsed without a type. */
    p = make_problem(&a, "void moveZeroes(int[] nums)", NULL);
    case_values(&p, "[0,1]\n=> [1,0]\n", NULL, pool, &args, &exp);

    /* With check:, the expected output has the checked parameter's type:
     * all of it for param, its first k elements for prefix. */
    p.check.mode = LC_RESULT_PARAM;
    case_values(&p, "[0,1]\n=> [1,0]\n", NULL, pool, &args, &exp);
    case_values(&p, "[0,1]\n=> 2\n", "d/tests.txt:1: expected output: "
                "column 1: expected int[], got 2", pool, &args, &exp);
    p = make_problem(&a, "int removeElement(int[] nums, int val)", NULL);
    p.check.mode = LC_RESULT_PREFIX;
    if (case_values(&p, "[3,2,3]\n3\n=> [2]\n", NULL, pool, &args,
                    &exp) == LC_OK) {
        CHECK_INT(exp->kind, LC_V_ARRAY);
        CHECK_INT(exp->len, 1);
    }
    case_values(&p, "[3,2,3]\n3\n=> 1\n", "d/tests.txt:1: expected "
                "output: column 1: expected int[], got 1", pool, &args, &exp);
    p = make_problem(&a, "void f(int k, char[][] g)", NULL);
    p.check.mode = LC_RESULT_PARAM;
    p.check.param = 1;
    case_values(&p, "1\n[[\"a\"]]\n=> [[\"b\"]]\n", NULL, pool, &args,
                &exp);
    case_values(&p, "1\n[[\"a\"]]\n=> [[1]]\n", "d/tests.txt:1: expected "
                "output: column 3: expected char, got 1", pool, &args, &exp);

    /* Without a signature everything is untyped, with no names. */
    p = make_problem(&a, NULL, NULL);
    case_values(&p, "[1,\"a\"]\ntrue\n=> null\n", NULL, pool, &args, &exp);
    case_values(&p, "1\nx\n=> 1\n", "d/tests.txt:1: input 2: column 1: "
                "unexpected character 'x'", pool, &args, &exp);
    case_values(&p, "1\n=> [1\n", "d/tests.txt:1: expected output: column "
                "1: '[' is never closed", pool, &args, &exp);

    {
        lc_ctx *ctx = lc_open(NULL);
        lc_case c;
        memset(&c, 0, sizeof c);
        CHECK_INT(lc_case_values(ctx, NULL, &c, pool, &args, &exp),
                  LC_ERR_INVALID);
        CHECK_INT(lc_case_values(ctx, &p, &c, NULL, &args, &exp),
                  LC_ERR_INVALID);
        lc_close(ctx);
    }
    lc_pool_free(pool);
    lc_arena_free(&a);
}

/* Every case of every pack in `dir` must fit its problem's signature. */
/* Cases parsed from a file remember it, and their errors name it. */
static void test_case_file(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_arena a = {0};
    lc_problem p = make_problem(&a, "int f(int x)", NULL);
    lc_pool *pool = lc_pool_new();
    lc_cases *cs = parse_ok(ctx, "1\n=> 2\n\n\"x\"\n=> 3\n");
    const lc_value *args, *exp;

    if (cs && cs->count == 2) {
        CHECK_STR(cs->items[0].file, "t.txt");
        CHECK_STR(cs->items[1].file, "t.txt");
        CHECK_INT(lc_case_values(ctx, &p, &cs->items[1], pool, &args, &exp),
                  LC_ERR_PARSE);
        CHECK_STR(lc_last_error(ctx),
                  "t.txt:4: x: column 1: expected int, got a string");
    }
    lc_cases_free(cs);
    lc_pool_free(pool);
    lc_arena_free(&a);
    lc_close(ctx);
}

static void expect_user_error(const char *text, const char *msg)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_cases *cs = NULL;

    write_file(TMP "/mine.txt", text);
    CHECK_INT(lc_load_user_cases(ctx, TMP "/mine.txt", &cs), LC_ERR_PARSE);
    CHECK(cs == NULL);
    CHECK_STR(lc_last_error(ctx), msg);
    lc_close(ctx);
}

/* A file of your own cases: '=> expected' may be left out, and then a
 * blank line or the end of the file ends the case. */
static void test_user_cases(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_arena a = {0};
    lc_problem p = make_problem(&a, "int[] f(int[] nums, int k)", NULL);
    lc_pool *pool = lc_pool_new();
    const lc_value *args, *exp;
    lc_cases *cs = NULL;
    int rc;

    write_file(TMP "/mine.txt",
               "# Mine.\n[1,2]\n3\n=> [0,1]\n\n[4,5]\n9\n\n\n  [6]\n"
               "# a comment inside a case\n7");
    rc = lc_load_user_cases(ctx, TMP "/mine.txt", &cs);
    CHECK_INT(rc, LC_OK);
    if (rc == LC_OK) {
        CHECK_INT(cs->count, 3);
        check_case(cs, 0, ARGS("[1,2]", "3"), "[0,1]", NULL, 0, 2);
        check_case(cs, 1, ARGS("[4,5]", "9"), NULL, NULL, 0, 6);
        check_case(cs, 2, ARGS("[6]", "7"), NULL, NULL, 0, 10);
        CHECK_STR(cs->items[2].file, TMP "/mine.txt");
        /* No expected result: only the inputs are parsed. */
        args = exp = NULL;
        CHECK_INT(lc_case_values(ctx, &p, &cs->items[1], pool, &args, &exp),
                  LC_OK);
        CHECK(args != NULL && exp == NULL);
    }
    lc_cases_free(cs);

    /* A blank line right after '=>' is just a blank line. */
    write_file(TMP "/mine.txt", "[1]\n2\n=> [0]\n\n\n");
    CHECK_INT(lc_load_user_cases(ctx, TMP "/mine.txt", &cs), LC_OK);
    if (cs)
        CHECK_INT(cs->count, 1);
    lc_cases_free(cs);
    cs = NULL;

    /* An empty file has no cases. */
    write_file(TMP "/mine.txt", "# nothing yet\n");
    CHECK_INT(lc_load_user_cases(ctx, TMP "/mine.txt", &cs), LC_OK);
    if (cs)
        CHECK_INT(cs->count, 0);
    lc_cases_free(cs);
    cs = NULL;

    CHECK_INT(lc_load_user_cases(ctx, TMP "/missing.txt", &cs),
              LC_ERR_NOTFOUND);
    CHECK(cs == NULL);
    CHECK_INT(lc_load_user_cases(ctx, TMP, &cs), LC_ERR_IO);
    CHECK_STR(lc_last_error(ctx), "cannot read '" TMP "': not a regular "
              "file");
    CHECK_INT(lc_load_user_cases(ctx, NULL, &cs), LC_ERR_INVALID);
    CHECK_INT(lc_load_user_cases(ctx, TMP "/mine.txt", NULL),
              LC_ERR_INVALID);

    expect_user_error("@example\n1\n=> 2\n",
                      TMP "/mine.txt:1: @example is only for a problem's "
                      "tests.txt");
    expect_user_error("1\n\n=> 2\n",
                      TMP "/mine.txt:3: '=>' with no input lines before it");
    expect_user_error("1\n=>\n", TMP "/mine.txt:2: nothing after '=>'");

    lc_pool_free(pool);
    lc_arena_free(&a);
    lc_close(ctx);
}

static void check_packs(const char *dir, size_t min_cases)
{
    lc_options opt;
    lc_ctx *ctx;
    lc_pool *pool = lc_pool_new();
    size_t i, j, total = 0;

    lc_options_init(&opt);
    opt.problems_dir = dir;
    ctx = lc_open(&opt);
    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_INT(lc_warning_count(ctx), 0);
    for (i = 0; i < lc_problem_count(ctx); i++) {
        const lc_problem *p = lc_problem_at(ctx, i);
        lc_cases *cs = NULL;
        int rc = lc_load_cases(ctx, p, &cs);

        CHECK_INT(rc, LC_OK);
        if (rc != LC_OK) {
            fprintf(stderr, "    %s\n", lc_last_error(ctx));
            continue;
        }
        for (j = 0; j < cs->count; j++) {
            const lc_value *args, *exp;
            rc = lc_case_values(ctx, p, &cs->items[j], pool, &args, &exp);
            CHECK_INT(rc, LC_OK);
            if (rc != LC_OK)
                fprintf(stderr, "    %s\n", lc_last_error(ctx));
            total++;
        }
        lc_cases_free(cs);
    }
    CHECK(total >= min_cases);
    lc_pool_free(pool);
    lc_close(ctx);
}

static void test_shipped_packs(void)
{
    check_packs("problems", 30);
    check_packs(FIXTURES, 5);
}

int main(void)
{
    /* Start from a clean scratch directory. */
    if (system("rm -rf " TMP) != 0)
        return 1;
    make_dir(TMP);
    RUN(test_spec_example);
    RUN(test_crlf_and_bom);
    RUN(test_trimming);
    RUN(test_comments_and_blanks);
    RUN(test_explanations);
    RUN(test_empty);
    RUN(test_nul_byte);
    RUN(test_errors);
    RUN(test_gen_directive);
    RUN(test_regen_mode);
    RUN(test_filename_and_misuse);
    RUN(test_many_cases);
    RUN(test_load_cases);
    RUN(test_no_inputs);
    RUN(test_case_values);
    RUN(test_case_file);
    RUN(test_user_cases);
    RUN(test_shipped_packs);
    return test_report();
}
