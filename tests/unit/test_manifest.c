#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* MANIFEST.tsv (manifest.c). */

#include <stdio.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static lc_manifest *parse(lc_ctx *ctx, const char *text)
{
    lc_manifest *m = NULL;
    int rc = lc_parse_manifest(ctx, text, strlen(text), "M.tsv", &m);

    CHECK_INT(rc, LC_OK);
    if (rc != LC_OK)
        fprintf(stderr, "    error: %s\n", lc_last_error(ctx));
    return m;
}

static void expect_error(const char *text, const char *msg)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_manifest *m = NULL;

    CHECK_INT(lc_parse_manifest(ctx, text, strlen(text), "M.tsv", &m),
              LC_ERR_PARSE);
    CHECK(m == NULL);
    CHECK_STR(lc_last_error(ctx), msg);
    lc_close(ctx);
}

static void test_rows(void)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_manifest *m = parse(ctx,
                           "\xEF\xBB\xBF# id\tslug\n"
                           "\n"
                           "1\ttwo-sum\tTwo Sum\teasy\tno\tunordered\tverified\n"
                           "175\tcombine-two-tables\tCombine Two Tables\teasy"
                           "\tno\tskip:sql\ttodo\r\n"
                           "204\t3sum\t3Sum: A Title\tmedium\tyes\tplain"
                           "\tdraft\n");
    const lc_manifest_row *r;

    CHECK(m != NULL);
    if (!m) {
        lc_close(ctx);
        return;
    }
    CHECK_STR(m->path, "M.tsv");
    CHECK_INT(m->count, 3);
    r = lc_manifest_find(m, 1);
    CHECK(r != NULL && r->line == 3);
    if (r) {
        CHECK_STR(r->slug, "two-sum");
        CHECK_STR(r->title, "Two Sum");
        CHECK_INT(r->difficulty, LC_EASY);
        CHECK_INT(r->premium, 0);
        CHECK_STR(r->group, "unordered");
        CHECK(r->skip == NULL);
        CHECK_STR(r->status, "verified");
    }
    r = lc_manifest_find(m, 175);
    CHECK(r != NULL);
    if (r) {
        CHECK_STR(r->group, "skip");
        CHECK_STR(r->skip, "sql");
        CHECK_STR(r->status, "todo");
    }
    r = lc_manifest_find(m, 204);
    CHECK(r != NULL);
    if (r) {
        CHECK_STR(r->title, "3Sum: A Title");
        CHECK_INT(r->difficulty, LC_MEDIUM);
        CHECK_INT(r->premium, 1);
    }
    CHECK(lc_manifest_find(m, 2) == NULL);
    CHECK(lc_manifest_find(NULL, 1) == NULL);
    lc_manifest_free(m);
    lc_manifest_free(NULL);

    m = parse(ctx, "");
    CHECK(m && m->count == 0);
    lc_manifest_free(m);
    lc_close(ctx);
}

static void test_errors(void)
{
    expect_error("1\ttwo-sum\tTwo Sum\teasy\tno\tplain\n",
                 "M.tsv:1: expected 7 tab-separated fields (id, slug, title, "
                 "difficulty, premium, group, status), got 6");
    expect_error("1\ttwo-sum\tTwo Sum\teasy\tno\tplain\tdraft\tx\n",
                 "M.tsv:1: expected 7 tab-separated fields (id, slug, title, "
                 "difficulty, premium, group, status), got 8");
    expect_error("01\ttwo-sum\tT\teasy\tno\tplain\tdraft\n",
                 "M.tsv:1: invalid id '01' (expected an integer from 1 to "
                 "99999)");
    expect_error("100000\ttwo-sum\tT\teasy\tno\tplain\tdraft\n",
                 "M.tsv:1: invalid id '100000' (expected an integer from 1 to "
                 "99999)");
    expect_error("2\ta\tA\teasy\tno\tplain\tdraft\n"
                 "1\tb\tB\teasy\tno\tplain\tdraft\n",
                 "M.tsv:2: id 1 is out of order (rows must be in increasing "
                 "id order)");
    expect_error("2\ta\tA\teasy\tno\tplain\tdraft\n"
                 "2\tb\tB\teasy\tno\tplain\tdraft\n",
                 "M.tsv:2: id 2 is listed twice (rows must be in increasing "
                 "id order)");
    expect_error("1\tTwo-Sum\tT\teasy\tno\tplain\tdraft\n",
                 "M.tsv:1: invalid slug 'Two-Sum'");
    expect_error("1\ttwo--sum\tT\teasy\tno\tplain\tdraft\n",
                 "M.tsv:1: invalid slug 'two--sum'");
    expect_error("1\ttwo-sum\t\teasy\tno\tplain\tdraft\n",
                 "M.tsv:1: empty title");
    expect_error("1\ttwo-sum\tT\tEasy\tno\tplain\tdraft\n",
                 "M.tsv:1: invalid difficulty 'Easy' (expected easy, medium "
                 "or hard)");
    expect_error("1\ttwo-sum\tT\teasy\tmaybe\tplain\tdraft\n",
                 "M.tsv:1: invalid premium 'maybe' (expected yes or no)");
    expect_error("1\ttwo-sum\tT\teasy\tno\tgraph\tdraft\n",
                 "M.tsv:1: unknown group 'graph' (expected one of plain, "
                 "bits, float, inplace, unordered, list, list-builder, tree, "
                 "tree-ref, nary, design, api, checker, extend, or "
                 "skip:REASON)");
    expect_error("1\ttwo-sum\tT\teasy\tno\tskip:\tdraft\n",
                 "M.tsv:1: unknown group 'skip:' (expected one of plain, "
                 "bits, float, inplace, unordered, list, list-builder, tree, "
                 "tree-ref, nary, design, api, checker, extend, or "
                 "skip:REASON)");
    expect_error("1\ttwo-sum\tT\teasy\tno\tskip:No SQL\tdraft\n",
                 "M.tsv:1: invalid skip reason 'No SQL' (use lowercase "
                 "letters, digits and dashes)");
    expect_error("1\ttwo-sum\tT\teasy\tno\tplain\tdone\n",
                 "M.tsv:1: invalid status 'done' (expected todo, draft or "
                 "verified)");
    expect_error("1\ta\tA\teasy\tno\tplain\tdraft\n"
                 "# c\n"
                 "5\tb\tB\teasy\tno\tplain\tdraft\n"
                 "7\ta\tC\teasy\tno\tplain\tdraft\n",
                 "M.tsv:4: slug 'a' is already used on line 1");
}

/* lc_load_manifest reads MANIFEST.tsv from the problem directory. */
static void test_load(void)
{
    lc_options opt;
    lc_ctx *ctx;
    lc_manifest *m = (lc_manifest *)1;

    lc_options_init(&opt);
    opt.problems_dir = "tests/fixtures/cli";
    ctx = lc_open(&opt);
    CHECK_INT(lc_load_manifest(ctx, &m), LC_ERR_INVALID); /* not loaded */
    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_INT(lc_load_manifest(ctx, &m), LC_OK);
    CHECK(m == NULL); /* there is none */
    lc_close(ctx);

    opt.problems_dir = "tests/fixtures/problems";
    ctx = lc_open(&opt);
    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_INT(lc_load_manifest(ctx, &m), LC_OK);
    CHECK(m != NULL && m->count == 9);
    CHECK(m && lc_manifest_find(m, 252) && lc_manifest_find(m, 252)->premium);
    lc_manifest_free(m);
    lc_close(ctx);

    opt.problems_dir = "problems";
    ctx = lc_open(&opt);
    CHECK_INT(lc_load(ctx), LC_OK);
    CHECK_INT(lc_load_manifest(ctx, &m), LC_OK);
    CHECK(m != NULL);
    if (m) {
        size_t i;

        CHECK(strstr(m->path, "/problems/MANIFEST.tsv") != NULL);
        /* Every shipped pack is listed, as it is. */
        for (i = 0; i < lc_problem_count(ctx); i++) {
            const lc_problem *p = lc_problem_at(ctx, i);
            const lc_manifest_row *r = lc_manifest_find(m, p->id);

            CHECK(r != NULL);
            if (r) {
                CHECK_STR(r->slug, p->slug);
                CHECK_STR(r->title, p->title);
                CHECK_INT(r->difficulty, p->difficulty);
            }
        }
    }
    lc_manifest_free(m);
    lc_close(ctx);
}

int main(void)
{
    RUN(test_rows);
    RUN(test_errors);
    RUN(test_load);
    return test_report();
}
