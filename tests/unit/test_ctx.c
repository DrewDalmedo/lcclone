#include <errno.h>
#include <string.h>

#include "internal.h"
#include "test.h"

static void test_open_close(void)
{
    lc_options opt;
    lc_ctx *ctx;

    lc_options_init(&opt);
    CHECK(opt.problems_dir == NULL);
    opt.problems_dir = "/nonexistent";
    ctx = lc_open(&opt);
    CHECK(ctx != NULL);
    CHECK_STR(ctx->opt_problems_dir, "/nonexistent");
    CHECK_STR(lc_last_error(ctx), "");
    CHECK(lc_problems_dir(ctx) == NULL);
    CHECK_INT(lc_warning_count(ctx), 0);
    lc_close(ctx);
    lc_close(NULL);
    ctx = lc_open(NULL);
    CHECK(ctx != NULL);
    lc_close(ctx);
}

static void test_errors(void)
{
    lc_ctx *ctx = lc_open(NULL);

    CHECK_INT(lc_set_error(ctx, LC_ERR_PARSE, "bad %s at %d", "thing", 3),
              LC_ERR_PARSE);
    CHECK_STR(lc_last_error(ctx), "bad thing at 3");
    /* Arguments may point into the current message. */
    lc_set_error(ctx, LC_ERR_IO, "wrapped: %s", lc_last_error(ctx));
    CHECK_STR(lc_last_error(ctx), "wrapped: bad thing at 3");
    CHECK_INT(lc_set_errno(ctx, LC_ERR_IO, ENOENT, "cannot open '%s'", "f"),
              LC_ERR_IO);
    CHECK(strncmp(lc_last_error(ctx), "cannot open 'f': ", 17) == 0);
    CHECK_INT(lc_oom(ctx), LC_ERR_NOMEM);
    CHECK_STR(lc_last_error(ctx), "out of memory");
    lc_set_error(ctx, LC_ERR_INVALID, "fine again");
    CHECK_STR(lc_last_error(ctx), "fine again");
    lc_close(ctx);
}

static void test_warnings(void)
{
    lc_ctx *ctx = lc_open(NULL);
    int i;

    for (i = 0; i < 50; i++)
        CHECK_INT(lc_add_warning(ctx, "warning %d", i), LC_OK);
    CHECK_INT(lc_warning_count(ctx), 50);
    CHECK_STR(lc_warning_at(ctx, 0), "warning 0");
    CHECK_STR(lc_warning_at(ctx, 49), "warning 49");
    CHECK(lc_warning_at(ctx, 50) == NULL);
    lc_close(ctx);
}

static void test_difficulty(void)
{
    CHECK_STR(lc_difficulty_name(LC_EASY), "easy");
    CHECK_STR(lc_difficulty_name(LC_MEDIUM), "medium");
    CHECK_STR(lc_difficulty_name(LC_HARD), "hard");
    CHECK_STR(lc_difficulty_name(LC_DIFF_UNKNOWN), "unknown");
    CHECK_INT(lc_difficulty_from_name("Easy"), LC_EASY);
    CHECK_INT(lc_difficulty_from_name("MEDIUM"), LC_MEDIUM);
    CHECK_INT(lc_difficulty_from_name("hard"), LC_HARD);
    CHECK_INT(lc_difficulty_from_name("hardest"), LC_DIFF_UNKNOWN);
    CHECK_INT(lc_difficulty_from_name(""), LC_DIFF_UNKNOWN);
    CHECK_INT(lc_difficulty_from_name(NULL), LC_DIFF_UNKNOWN);
}

static void test_version(void)
{
    CHECK_STR(lc_version(), LC_VERSION);
}

int main(void)
{
    RUN(test_open_close);
    RUN(test_errors);
    RUN(test_warnings);
    RUN(test_difficulty);
    RUN(test_version);
    return test_report();
}
