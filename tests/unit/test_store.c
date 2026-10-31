#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

/* Your progress and submission history (store.c). */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "internal.h"
#include "test.h"

#define TMP "tests/tmp/store"
#define FIXTURES "tests/fixtures/problems"

static void write_file(const char *path, const char *data)
{
    FILE *f = fopen(path, "wb");

    CHECK(f != NULL);
    if (!f)
        return;
    CHECK(fwrite(data, 1, strlen(data), f) == strlen(data));
    CHECK(fclose(f) == 0);
}

/* The whole file, malloc'ed ("" when it can't be read). */
static char *read_all(const char *path)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_str s = LC_STR_INIT;
    char *copy;

    lc_read_file(ctx, path, LC_MIB, &s);
    copy = strdup(lc_str_cstr(&s));
    lc_str_free(&s);
    lc_close(ctx);
    return copy;
}

/* A context whose data directory is `dir`, emptied first. */
static lc_ctx *open_in(const char *dir, int load)
{
    lc_options opt;
    lc_ctx *ctx;
    char cmd[512];

    snprintf(cmd, sizeof cmd, "rm -rf '%s' && mkdir -p '%s'", dir, dir);
    CHECK(system(cmd) == 0);
    lc_options_init(&opt);
    opt.data_dir = dir;
    opt.problems_dir = FIXTURES;
    opt.config_file = "";
    ctx = lc_open(&opt);
    CHECK(ctx != NULL);
    if (load)
        CHECK_INT(lc_load(ctx), LC_OK);
    return ctx;
}

/* 1 if s ends with `end`. */
static int ends_with(const char *s, const char *end)
{
    size_t n = s ? strlen(s) : 0, m = strlen(end);

    return n >= m && strcmp(s + n - m, end) == 0;
}

/* Files in a directory, sorted, one per line. */
static void list_dir(const char *dir, char *out, size_t cap)
{
    lc_ctx *ctx = lc_open(NULL);
    lc_arena a = {NULL};
    char **names;
    size_t n, i;

    out[0] = '\0';
    if (lc_list_dir(ctx, dir, &a, &names, &n) == LC_OK)
        for (i = 0; i < n; i++) {
            strncat(out, names[i], cap - strlen(out) - 2);
            strcat(out, "\n");
        }
    lc_arena_free(&a);
    lc_close(ctx);
}

static void test_status_names(void)
{
    CHECK_STR(lc_status_name(LC_TODO), "todo");
    CHECK_STR(lc_status_name(LC_ATTEMPTED), "attempted");
    CHECK_STR(lc_status_name(LC_SOLVED), "solved");
    CHECK_INT(lc_status_from_name("todo"), LC_TODO);
    CHECK_INT(lc_status_from_name("attempted"), LC_ATTEMPTED);
    CHECK_INT(lc_status_from_name("solved"), LC_SOLVED);
    CHECK_INT(lc_status_from_name("Solved"), -1);
    CHECK_INT(lc_status_from_name(NULL), -1);
}

static void test_read(void)
{
    lc_ctx *ctx = open_in(TMP "/read", 0);
    lc_progress_entry e;

    /* No progress.tsv: nothing submitted. */
    CHECK_INT(lc_progress(ctx, 1, &e), LC_OK);
    CHECK_INT(e.id, 1);
    CHECK_INT(e.status, LC_TODO);
    CHECK_STR(e.lang, "");
    CHECK_INT(e.best_ms, -1);
    CHECK_INT(e.last_ts, 0);
    CHECK_INT(lc_warning_count(ctx), 0);
    lc_close(ctx);

    ctx = open_in(TMP "/read", 0);
    write_file(TMP "/read/progress.tsv",
               "# a comment\n"
               "1\tsolved\tcpp\t12\t1700000000\n"
               "\n"
               "9\tattempted\tc\t-\t1700000100\textra\tcolumns\n"
               "20\tsolved\tc\t7\t1700000200\r\n"
               "bad line\n"
               "70\tfinished\tc\t1\t1\n"
               "71\tsolved\trust\t1\t1\n"
               "72\tsolved\tc\tfast\t1\n"
               "73\tsolved\tc\t1\tyesterday\n"
               "0\tsolved\tc\t1\t1\n"
               "20\tattempted\tcpp\t-\t1700000300\n");
    CHECK_INT(lc_progress(ctx, 1, &e), LC_OK);
    CHECK_INT(e.status, LC_SOLVED);
    CHECK_STR(e.lang, "cpp");
    CHECK_INT(e.best_ms, 12);
    CHECK_INT(e.last_ts, 1700000000);
    CHECK_INT(lc_progress(ctx, 9, &e), LC_OK);
    CHECK_INT(e.status, LC_ATTEMPTED);
    CHECK_STR(e.lang, "c");
    CHECK_INT(e.best_ms, -1);
    /* The last line of a problem counts. */
    CHECK_INT(lc_progress(ctx, 20, &e), LC_OK);
    CHECK_INT(e.status, LC_ATTEMPTED);
    CHECK_INT(e.last_ts, 1700000300);
    CHECK_INT(lc_progress(ctx, 70, &e), LC_OK);
    CHECK_INT(e.status, LC_TODO);
    CHECK_INT(lc_warning_count(ctx), 6);
    CHECK(ends_with(lc_warning_at(ctx, 0), TMP "/read/progress.tsv:6: expected 5 "
              "tab-separated fields; line skipped"));
    CHECK(ends_with(lc_warning_at(ctx, 1), TMP "/read/progress.tsv:7: bad status "
              "(expected attempted or solved); line skipped"));
    CHECK(ends_with(lc_warning_at(ctx, 2), TMP "/read/progress.tsv:8: bad lang "
              "(expected c, cpp or -); line skipped"));
    CHECK(ends_with(lc_warning_at(ctx, 3), TMP "/read/progress.tsv:9: bad best_ms; "
              "line skipped"));
    CHECK(ends_with(lc_warning_at(ctx, 4), TMP "/read/progress.tsv:10: bad "
              "last_ts; line skipped"));
    CHECK(ends_with(lc_warning_at(ctx, 5), TMP "/read/progress.tsv:11: bad id; "
              "line skipped"));
    /* Read once: a later change isn't seen by this context. */
    write_file(TMP "/read/progress.tsv", "1\tattempted\tc\t-\t1\n");
    CHECK_INT(lc_progress(ctx, 1, &e), LC_OK);
    CHECK_INT(e.status, LC_SOLVED);
    lc_close(ctx);

    /* A progress.tsv that can't be read is a warning, not a failure. */
    ctx = open_in(TMP "/read", 0);
    CHECK(mkdir(TMP "/read/progress.tsv", 0777) == 0);
    CHECK_INT(lc_progress(ctx, 1, &e), LC_OK);
    CHECK_INT(e.status, LC_TODO);
    CHECK_INT(lc_warning_count(ctx), 1);
    CHECK(strstr(lc_warning_at(ctx, 0), "; your progress is not shown") !=
          NULL);
    lc_close(ctx);
}

/* A report as lc_judge would give for a submission. */
static lc_report report(lc_verdict v, long max_ms)
{
    lc_report r;

    memset(&r, 0, sizeof r);
    r.verdict = v;
    r.max_time_ms = max_ms;
    return r;
}

static void test_record(void)
{
    lc_ctx *ctx = open_in(TMP "/rec", 1);
    const lc_problem *p = lc_problem_find(ctx, "two-sum");
    const lc_lang *c = lc_lang_find("c"), *cpp = lc_lang_find("cpp");
    lc_record_info info;
    lc_report r;
    lc_progress_entry e;
    char *text, files[1024];
    const char *src = TMP "/rec/solution.c";

    CHECK(p != NULL);
    if (!p) {
        lc_close(ctx);
        return;
    }
    write_file(src, "int *twoSum(void);\n");

    /* A wrong answer: attempted, and a copy in the history. */
    r = report(LC_WRONG_ANSWER, 5);
    CHECK_INT(lc_record(ctx, p, c, src, &r, 0, &info), LC_OK);
    CHECK_INT(info.recorded, 1);
    CHECK_INT(info.before.status, LC_TODO);
    CHECK_INT(info.after.status, LC_ATTEMPTED);
    CHECK_STR(info.after.lang, "c");
    CHECK_INT(info.after.best_ms, -1);
    CHECK(info.after.last_ts > 1700000000);
    CHECK(strstr(info.saved.data, TMP "/rec/submissions/0001/") != NULL);
    CHECK(strstr(info.saved.data, "Z-wrong_answer.c") != NULL);
    text = read_all(info.saved.data);
    CHECK_STR(text, "int *twoSum(void);\n");
    free(text);
    lc_record_info_free(&info);
    /* What lc_progress says follows. */
    CHECK_INT(lc_progress(ctx, 1, &e), LC_OK);
    CHECK_INT(e.status, LC_ATTEMPTED);

    /* Accepted in C++: solved, with its time. */
    r = report(LC_ACCEPTED, 30);
    CHECK_INT(lc_record(ctx, p, cpp, src, &r, 0, &info), LC_OK);
    CHECK_INT(info.before.status, LC_ATTEMPTED);
    CHECK_INT(info.after.status, LC_SOLVED);
    CHECK_STR(info.after.lang, "cpp");
    CHECK_INT(info.after.best_ms, 30);
    CHECK(strstr(info.saved.data, "-accepted.cpp") != NULL);
    lc_record_info_free(&info);
    /* Faster: the best time; slower or with sanitizers: kept. */
    r = report(LC_ACCEPTED, 20);
    CHECK_INT(lc_record(ctx, p, c, src, &r, 0, &info), LC_OK);
    CHECK_INT(info.after.best_ms, 20);
    CHECK_STR(info.after.lang, "c");
    lc_record_info_free(&info);
    r = report(LC_ACCEPTED, 40);
    CHECK_INT(lc_record(ctx, p, cpp, src, &r, 0, &info), LC_OK);
    CHECK_INT(info.after.best_ms, 20);
    lc_record_info_free(&info);
    r = report(LC_ACCEPTED, 1);
    CHECK_INT(lc_record(ctx, p, c, src, &r, 1, &info), LC_OK);
    CHECK_INT(info.after.best_ms, 20);
    lc_record_info_free(&info);
    /* A failure later leaves it solved, in the language that solved it. */
    r = report(LC_COMPILE_ERROR, 0);
    CHECK_INT(lc_record(ctx, p, cpp, src, &r, 0, &info), LC_OK);
    CHECK_INT(info.after.status, LC_SOLVED);
    CHECK_STR(info.after.lang, "c");
    CHECK(strstr(info.saved.data, "-compile_error.cpp") != NULL);
    lc_record_info_free(&info);

    /* Six submissions, all kept: those in the same second numbered. */
    list_dir(TMP "/rec/submissions/0001", files, sizeof files);
    {
        int n = 0;
        char *q;

        for (q = files; (q = strchr(q, '\n')) != NULL; q++)
            n++;
        CHECK_INT(n, 6);
    }
    text = read_all(TMP "/rec/progress.tsv");
    CHECK(strncmp(text, "# lcclone progress: ", 20) == 0);
    CHECK(strstr(text, "\n1\tsolved\tc\t20\t") != NULL);
    free(text);

    /* A cancelled run is not a submission. */
    r = report(LC_VERDICT_NONE, 0);
    r.cancelled = 1;
    CHECK_INT(lc_record(ctx, p, c, src, &r, 0, &info), LC_ERR_INVALID);
    lc_record_info_free(&info);
    /* A missing solution: progress is recorded, the copy isn't. */
    r = report(LC_WRONG_ANSWER, 1);
    CHECK_INT(lc_record(ctx, lc_problem_find(ctx, "4"), c, TMP "/nope.c", &r,
                        0, &info),
              LC_ERR_NOTFOUND);
    CHECK_INT(info.after.status, LC_ATTEMPTED);
    CHECK_INT(info.saved.len, 0);
    lc_record_info_free(&info);
    CHECK_INT(lc_progress(ctx, 4, &e), LC_OK);
    CHECK_INT(e.status, LC_ATTEMPTED);
    /* progress.tsv can't be replaced: nothing is recorded. */
    CHECK(rename(TMP "/rec/progress.tsv", TMP "/rec/moved") == 0);
    CHECK(mkdir(TMP "/rec/progress.tsv", 0777) == 0);
    r = report(LC_ACCEPTED, 1);
    CHECK(lc_record(ctx, lc_problem_find(ctx, "141"), c, src, &r, 0, &info)
          != LC_OK);
    CHECK_INT(info.recorded, 0);
    CHECK_INT(info.after.status, LC_TODO);
    CHECK_INT(info.saved.len, 0);
    lc_record_info_free(&info);
    lc_close(ctx);
}

/* Recording rewrites only its problem's line, in id order. */
static void test_rewrite(void)
{
    lc_ctx *ctx = open_in(TMP "/rw", 1);
    lc_record_info info;
    lc_report r = report(LC_ACCEPTED, 3);
    char *text;
    const char *src = TMP "/rw/s.c";

    write_file(src, "x\n");
    write_file(TMP "/rw/progress.tsv",
               "# mine\n"
               "1\tattempted\tc\t-\t5\tfuture\n"
               "junk line\n"
               "650\tsolved\tc\t9\t6\n"
               "1\tattempted\tc\t-\t4\n");
    CHECK_INT(lc_record(ctx, lc_problem_find(ctx, "1"), lc_lang_find("c"),
                        src, &r, 0, &info), LC_OK);
    lc_record_info_free(&info);
    CHECK_INT(lc_record(ctx, lc_problem_find(ctx, "141"), lc_lang_find("c"),
                        src, &r, 0, &info), LC_OK);
    lc_record_info_free(&info);
    CHECK_INT(lc_record(ctx, lc_problem_find(ctx, "2667"), lc_lang_find("c"),
                        src, &r, 0, &info), LC_OK);
    lc_record_info_free(&info);
    text = read_all(TMP "/rw/progress.tsv");
    {
        /* Times vary; compare the lines without them. */
        char *line, *save = NULL, buf[2048] = "";

        for (line = strtok_r(text, "\n", &save); line;
             line = strtok_r(NULL, "\n", &save)) {
            char *last = strrchr(line, '\t');
            if (last && strncmp(line, "#", 1) != 0 &&
                strcmp(line, "junk line") != 0 &&
                strstr(line, "future") == NULL)
                *last = '\0';
            strcat(buf, line);
            strcat(buf, "|");
        }
        CHECK_STR(buf, "# mine|1\tsolved\tc\t3|junk line|141\tsolved\tc\t3|"
                       "650\tsolved\tc\t9|2667\tsolved\tc\t3|");
    }
    free(text);
    lc_close(ctx);
}

static void test_stats(void)
{
    lc_ctx *ctx = open_in(TMP "/stats", 1);
    lc_stats_summary s;
    size_t i;
    int found = 0;

    write_file(TMP "/stats/progress.tsv",
               "1\tsolved\tc\t1\t1\n"
               "4\tattempted\tc\t-\t1\n"
               "650\tsolved\tcpp\t2\t1\n"
               "9999\tsolved\tc\t1\t1\n");
    CHECK_INT(lc_stats(ctx, &s), LC_OK);
    CHECK_INT(s.all.total, 6);
    CHECK_INT(s.all.solved, 2);
    CHECK_INT(s.all.attempted, 1);
    CHECK_STR(s.difficulty[LC_EASY].name, "easy");
    CHECK_INT(s.difficulty[LC_EASY].total, 3);
    CHECK_INT(s.difficulty[LC_EASY].solved, 1);
    CHECK_INT(s.difficulty[LC_MEDIUM].total, 2);
    CHECK_INT(s.difficulty[LC_MEDIUM].solved, 1);
    CHECK_INT(s.difficulty[LC_HARD].total, 1);
    CHECK_INT(s.difficulty[LC_HARD].attempted, 1);
    for (i = 1; i < s.ntags; i++)
        CHECK(strcmp(s.tags[i - 1].name, s.tags[i].name) < 0);
    for (i = 0; i < s.ntags; i++)
        if (strcmp(s.tags[i].name, "dynamic-programming") == 0) {
            found = 1;
            CHECK_INT(s.tags[i].total, 1);
            CHECK_INT(s.tags[i].solved, 1);
        }
    CHECK(found);
    lc_stats_free(&s);
    lc_stats_free(&s);
    lc_close(ctx);
}

int main(void)
{
    RUN(test_status_names);
    RUN(test_read);
    RUN(test_record);
    RUN(test_rewrite);
    RUN(test_stats);
    return test_report();
}
