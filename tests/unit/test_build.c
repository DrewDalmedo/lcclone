/*
 * test_build.c: building and judging real solutions with the system's
 * compilers ($CC and $CXX, else cc and c++): every type through both
 * runtimes, the stubs of every codegen fixture, the build cache, events,
 * compile errors, the reference solution, the standards and the errors
 * lc_judge reports. The judge fixtures are in tests/fixtures/judge.
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "internal.h"
#include "test.h"

#define JUDGE "tests/fixtures/judge"
#define CODEGEN "tests/fixtures/codegen"
#define TMP "tests/tmp/build"
#define CACHE TMP "/cache"
/* Where problem 10's C solutions are built. */
#define ADD_C CACHE "/build/0010-add/c-c11"

static void write_file(const char *path, const char *data)
{
    FILE *f = fopen(path, "w");

    CHECK(f != NULL);
    if (f) {
        fputs(data, f);
        fclose(f);
    }
}

/* How many entries of `dir` are named `prefix`...`suffix`; the path of
 * the last one goes in `path` (of `size` bytes) unless it is NULL. */
static int entries(const char *dir, const char *prefix, const char *suffix,
                   char *path, size_t size)
{
    DIR *d = opendir(dir);
    struct dirent *e;
    size_t pl = strlen(prefix), sl = strlen(suffix);
    int n = 0;

    if (!d)
        return 0;
    while ((e = readdir(d)) != NULL) {
        size_t l = strlen(e->d_name);

        if (l >= pl + sl && strncmp(e->d_name, prefix, pl) == 0 &&
            strcmp(e->d_name + l - sl, suffix) == 0) {
            n++;
            if (path && snprintf(path, size, "%s/%s", dir, e->d_name) >=
                            (int)size)
                path[0] = '\0';
        }
    }
    closedir(d);
    return n;
}

/* Makes a file look unused since 2000. */
static void make_old(const char *path)
{
    struct timespec t[2];

    t[0].tv_sec = t[1].tv_sec = 946684800;
    t[0].tv_nsec = t[1].tv_nsec = 0;
    CHECK(utimensat(AT_FDCWD, path, t, AT_SYMLINK_NOFOLLOW) == 0);
}

static lc_ctx *open_ctx(const char *problems)
{
    lc_options opt;
    lc_ctx *ctx;

    lc_options_init(&opt);
    opt.problems_dir = problems;
    opt.cache_dir = CACHE;
    ctx = lc_open(&opt);
    CHECK_INT(lc_load(ctx), LC_OK);
    return ctx;
}

/* The events of a run, as letters: b build start, B build done (r for
 * the reference), s case start, d case done, E done. */
typedef struct events {
    char log[256];
    int cached;               /* the last build was cached */
    int ok;                   /* the last build compiled */
} events;

static void on_event(void *ud, const lc_event *ev)
{
    events *e = ud;
    size_t n = strlen(e->log);
    char ch = '?';

    switch (ev->kind) {
    case LC_EV_BUILD_START:
        ch = ev->build->reference ? 'r' : 'b';
        break;
    case LC_EV_BUILD_DONE:
        ch = ev->build->reference ? 'R' : 'B';
        e->cached = ev->build->cached;
        e->ok = ev->build->ok;
        break;
    case LC_EV_CASE_START:
        ch = 's';
        break;
    case LC_EV_CASE_DONE:
        ch = 'd';
        break;
    case LC_EV_DONE:
        ch = 'E';
        break;
    case LC_EV_FINDING:
        ch = 'F'; /* only lc_check sends these */
        break;
    }
    if (n + 1 < sizeof e->log)
        e->log[n] = ch;
}

/* Judges `src` (a path under JUDGE) and returns the report, or NULL with
 * the error in `ctx`. */
static lc_report *judge(lc_ctx *ctx, const char *problem, const char *src,
                        const char *std, int all, events *ev)
{
    const lc_problem *p = lc_problem_find(ctx, problem);
    lc_judge_options opts;
    lc_hooks hooks;
    lc_report *rep = NULL;
    char path[256];

    CHECK(p != NULL);
    if (!p)
        return NULL;
    snprintf(path, sizeof path, "%s/%s", p->dir, src);
    memset(&opts, 0, sizeof opts);
    opts.std = std;
    opts.examples = 1;
    opts.all = all;
    memset(&hooks, 0, sizeof hooks);
    if (ev) {
        memset(ev, 0, sizeof *ev);
        hooks.on_event = on_event;
        hooks.ud = ev;
    }
    if (lc_judge(ctx, p, lc_lang_for_path(src), path, &opts, &hooks, &rep) !=
        LC_OK)
        return NULL;
    return rep;
}

/* Every case of `problem` passes with `src`. */
static void expect_accepted(lc_ctx *ctx, const char *problem,
                            const char *src, const char *std)
{
    lc_report *rep = judge(ctx, problem, src, std, 1, NULL);

    CHECK(rep != NULL);
    if (!rep) {
        fprintf(stderr, "    %s/%s: %s\n", problem, src, lc_last_error(ctx));
        return;
    }
    CHECK_INT(rep->verdict, LC_ACCEPTED);
    CHECK(rep->total > 0 && rep->passed == rep->total);
    if (rep->verdict != LC_ACCEPTED) {
        size_t i;

        fprintf(stderr, "    %s/%s (%s): %s\n", problem, src,
                std ? std : "default", lc_verdict_name(rep->verdict));
        if (rep->build && !rep->build->ok)
            fprintf(stderr, "%s", rep->build->output);
        for (i = 0; i < rep->nresults; i++)
            if (rep->results[i].verdict != LC_ACCEPTED)
                fprintf(stderr, "    line %d: got %s, expected %s; %s %s\n",
                        rep->results[i].line,
                        rep->results[i].output ? rep->results[i].output
                                               : "nothing",
                        rep->results[i].expected,
                        rep->results[i].message ? rep->results[i].message
                                                : "",
                        rep->results[i].stderr_text);
    }
    lc_report_free(rep);
}

/*
 * Every type, in and out, through both runtimes. C++ builds are slow
 * (most of it is the prelude's headers), so only the packs whose types
 * lean hardest on the C++ runtime's overloads and templates also run at
 * the newest standard; test_stubs_compile builds every type with it.
 */
static void test_types(void)
{
    static const struct {
        const char *pack;
        int cpp;                  /* 0 none, 1 default std, 2 also max */
    } packs[] = {{"1", 1}, {"2", 1}, {"3", 1}, {"4", 2}, {"5", 1}, {"6", 1},
                 {"7", 2}, {"8", 1}, {"9", 2}, {"10", 1}, {"11", 0},
                 {"15", 1}, {"16", 2}, {"17", 1}, {"18", 1}};
    lc_ctx *ctx = open_ctx(JUDGE);
    size_t i;

    for (i = 0; i < sizeof packs / sizeof packs[0]; i++) {
        expect_accepted(ctx, packs[i].pack, "solution.c", NULL);
        if (packs[i].cpp >= 1)
            expect_accepted(ctx, packs[i].pack, "solution.cpp", NULL);
        if (packs[i].cpp >= 2)
            expect_accepted(ctx, packs[i].pack, "solution.cpp", "max");
    }
    expect_accepted(ctx, "9", "solution.c", "max");
    lc_close(ctx);
}

/* The stub of every fixture that lc can judge compiles with its harness,
 * in C and C++, at the default standard and the newest (C++ at the newest
 * only for the first three fixtures, which have every type). */
static void test_stubs_compile(void)
{
    lc_ctx *ctx = open_ctx(CODEGEN);
    size_t i, l, s;

    CHECK(system("mkdir -p " TMP "/stubs") == 0);
    for (i = 0; i < lc_problem_count(ctx); i++) {
        const lc_problem *p = lc_problem_at(ctx, i);

        if (lc_codegen_check(ctx, p) != LC_OK)
            continue;
        for (l = 0; l < lc_lang_count(); l++) {
            const lc_lang *lang = lc_lang_at(l);
            lc_str stub = LC_STR_INIT;
            char path[256];

            snprintf(path, sizeof path, TMP "/stubs/%d.%s", p->id,
                     lang->ext);
            CHECK_INT(lc_stub(ctx, p, lang, &stub), LC_OK);
            write_file(path, lc_str_cstr(&stub));
            lc_str_free(&stub);
            for (s = 0; s < (lang->id == LC_LANG_C || p->id <= 3 ? 2 : 1);
                 s++) {
                lc_arena a = {NULL};
                lc_compiler cc;
                lc_build_req req;
                lc_build b;
                const char *exe;
                int cancelled, rc;

                memset(&req, 0, sizeof req);
                CHECK_INT(lc_compiler_get(ctx, lang, &a, &cc), LC_OK);
                req.p = p;
                req.lang = lang;
                req.cc = &cc;
                req.src = path;
                CHECK_INT(lc_std_pick(ctx, lang, &cc, s ? "max" : NULL,
                                      &req.std, &req.std_flag), LC_OK);
                CHECK_INT(lc_build_init(ctx, &req, &a, &b), LC_OK);
                rc = lc_build_source(ctx, &req, &a, &b, &exe, &cancelled);
                CHECK_INT(rc, LC_OK);
                CHECK(b.ok && exe != NULL);
                if (rc == LC_OK && !b.ok)
                    fprintf(stderr, "    %s (%s):\n%s", path, req.std,
                            b.output);
                lc_arena_free(&a);
            }
        }
    }
    lc_close(ctx);
}

static void test_cache_and_events(void)
{
    lc_ctx *ctx = open_ctx(JUDGE);
    lc_report *rep;
    events ev;
    struct stat st;

    CHECK(system("rm -rf " TMP "/0010-add && mkdir -p " TMP "/0010-add") == 0);
    write_file(TMP "/0010-add/solution.c",
               "int add(int a, int b) { return a + b; }\n");

    /* A new build, then the same one from the cache. */
    rep = judge(ctx, "add", "../../../tmp/build/0010-add/solution.c", NULL, 0,
                &ev);
    CHECK(rep != NULL);
    if (rep) {
        CHECK_STR(ev.log, "bBsdsdE");
        CHECK(!ev.cached && ev.ok);
        CHECK(rep->build != NULL && rep->build->ok && !rep->build->cached);
        CHECK(rep->build && rep->build->source[0] == '/');
        CHECK_STR(rep->build ? rep->build->std : NULL, "c11");
        CHECK_STR(rep->build ? rep->build->lang : NULL, "c");
        CHECK_INT(rep->total, 2);
        CHECK_INT(rep->verdict, LC_ACCEPTED);
    }
    lc_report_free(rep);
    rep = judge(ctx, "add", "../../../tmp/build/0010-add/solution.c", NULL, 0,
                &ev);
    CHECK(rep && rep->build && rep->build->cached && ev.cached);
    CHECK(rep && rep->build && rep->build->time_ms == 0);
    lc_report_free(rep);
    CHECK(stat(CACHE "/build/0010-add/c-c11/main.c", &st) == 0);

    /* Any change to the source is a new build. */
    write_file(TMP "/0010-add/solution.c",
               "int add(int a, int b) { return b + a; }\n");
    rep = judge(ctx, "add", "../../../tmp/build/0010-add/solution.c", NULL, 0,
                &ev);
    CHECK(rep && rep->build && !rep->build->cached);
    lc_report_free(rep);

    /* Cases of your own without an expected result: the reference is
     * built after the solution, and runs before the cases. */
    {
        const lc_problem *p = lc_problem_find(ctx, "add");
        lc_cases *mine = NULL;
        lc_judge_options opts;
        lc_hooks hooks;

        CHECK_INT(lc_load_user_cases(ctx, JUDGE "/0010-add/mycases.txt",
                                     &mine), LC_OK);
        memset(&opts, 0, sizeof opts);
        opts.examples = 1;
        opts.cases = mine;
        memset(&hooks, 0, sizeof hooks);
        memset(&ev, 0, sizeof ev);
        hooks.on_event = on_event;
        hooks.ud = &ev;
        rep = NULL;
        CHECK_INT(lc_judge(ctx, p, lc_lang_find("c"),
                           JUDGE "/0010-add/wrong.c", &opts, &hooks, &rep),
                  LC_OK);
        CHECK_STR(ev.log, "bBrRsdsdsdsdsdE");
        if (rep) {
            CHECK_INT(rep->total, 5);
            CHECK_INT(rep->nresults, 5);
            CHECK_INT(rep->verdict, LC_WRONG_ANSWER);
            CHECK(!rep->results[1].custom && rep->results[2].custom);
            CHECK_INT(rep->results[2].line, 2);
            CHECK_STR(rep->results[3].expected, "30");
            CHECK_STR(rep->results[4].expected, "-7");
            CHECK_STR(rep->results[4].output, "1");
            CHECK_INT(rep->results[4].verdict, LC_WRONG_ANSWER);
            CHECK(strstr(rep->results[4].file, "/mycases.txt") != NULL);
        }
        lc_report_free(rep);
        lc_cases_free(mine);
    }
    lc_close(ctx);
}

/*
 * A cached build has the output and messages of the build that made the
 * program, kept in prog-<key>.log. Old programs go with their .log and
 * .dSYM.
 */
static void test_cached_output(void)
{
    lc_ctx *ctx = open_ctx(JUDGE);
    lc_report *rep;
    char first[4096] = "", log[512] = "";
    size_t i;
    int found = 0;

    CHECK(system("rm -rf " ADD_C) == 0);
    rep = judge(ctx, "add", "warn.c", NULL, 0, NULL);
    CHECK(rep && rep->build && rep->build->ok && !rep->build->cached);
    CHECK(rep && rep->build && rep->build->output_len > 0 &&
          rep->build->ndiags > 0);
    if (rep && rep->build)
        snprintf(first, sizeof first, "%s", rep->build->output);
    lc_report_free(rep);
    CHECK_INT(entries(ADD_C, "prog-", ".log", log, sizeof log), 1);

    rep = judge(ctx, "add", "warn.c", NULL, 0, NULL);
    CHECK(rep && rep->build && rep->build->cached);
    if (rep && rep->build) {
        CHECK_INT(rep->build->time_ms, 0);
        CHECK_STR(rep->build->output, first);
        CHECK_INT(rep->build->output_len, strlen(first));
        for (i = 0; i < rep->build->ndiags; i++)
            if (rep->build->diags[i].in_solution &&
                rep->build->diags[i].line == 4 &&
                strcmp(rep->build->diags[i].severity, "warning") == 0)
                found = 1;
        CHECK(found);
    }
    lc_report_free(rep);

    /* A program without its .log (from an older lc) is built again. */
    CHECK(unlink(log) == 0);
    rep = judge(ctx, "add", "warn.c", NULL, 0, NULL);
    CHECK(rep && rep->build && !rep->build->cached &&
          rep->build->ndiags > 0);
    lc_report_free(rep);
    CHECK_INT(entries(ADD_C, "prog-", ".log", NULL, 0), 1);
    rep = judge(ctx, "add", "warn.c", NULL, 0, NULL);
    CHECK(rep && rep->build && rep->build->cached);
    lc_report_free(rep);

    /* A new build removes the programs unused for ten minutes with their
     * .log and .dSYM, and the .dSYM an interrupted build left. */
    CHECK(system("mkdir -p " ADD_C "/prog-0123456789abcdef.dSYM/Contents/"
                 "Resources/DWARF " ADD_C "/prog-0123456789abcdef.tmp-1-1."
                 "dSYM/Contents") == 0);
    write_file(ADD_C "/prog-0123456789abcdef", "");
    write_file(ADD_C "/prog-0123456789abcdef.log", "");
    write_file(ADD_C "/prog-0123456789abcdef.dSYM/Contents/Resources/DWARF/"
               "prog-0123456789abcdef", "");
    make_old(ADD_C "/prog-0123456789abcdef");
    make_old(ADD_C "/prog-0123456789abcdef.log");
    make_old(ADD_C "/prog-0123456789abcdef.dSYM");
    make_old(ADD_C "/prog-0123456789abcdef.tmp-1-1.dSYM");
    CHECK(system("mkdir -p " TMP "/0010-add") == 0);
    write_file(TMP "/0010-add/fresh.c",
               "int add(int a, int b) { return a + b; } /* fresh */\n");
    rep = judge(ctx, "add", "../../../tmp/build/0010-add/fresh.c", NULL, 0,
                NULL);
    CHECK(rep && rep->build && rep->build->ok && !rep->build->cached);
    lc_report_free(rep);
    CHECK_INT(entries(ADD_C, "prog-0123456789abcdef", "", NULL, 0), 0);
    /* warn.c's program was used just now, so it stays. */
    CHECK_INT(entries(ADD_C, "prog-", ".log", NULL, 0), 2);
    lc_close(ctx);
}

static void test_compile_error(void)
{
    lc_ctx *ctx = open_ctx(JUDGE);
    lc_report *rep;
    events ev;
    size_t i;
    int found = 0;

    rep = judge(ctx, "add", "ce.c", NULL, 1, &ev);
    CHECK(rep != NULL);
    if (rep) {
        CHECK_STR(ev.log, "bBE");
        CHECK_INT(rep->verdict, LC_COMPILE_ERROR);
        CHECK_INT(rep->nresults, 0);
        CHECK_INT(rep->total, 5);
        CHECK(!rep->cancelled);
        CHECK(rep->build && !rep->build->ok && rep->build->output_len > 0);
        for (i = 0; rep->build && i < rep->build->ndiags; i++)
            if (rep->build->diags[i].in_solution &&
                rep->build->diags[i].line == 2 &&
                strcmp(rep->build->diags[i].severity, "error") == 0)
                found = 1;
        CHECK(found);
        CHECK_STR(rep->build ? rep->build->prototype : NULL,
                  "int add(int a, int b)");
    }
    lc_report_free(rep);

    /* A signature that doesn't match: the error is in the harness, and
     * the build says what the harness calls. */
    rep = judge(ctx, "add", "sig.cpp", NULL, 1, NULL);
    found = 0;
    for (i = 0; rep && rep->build && i < rep->build->ndiags; i++)
        if (strcmp(rep->build->diags[i].file, LC_HARNESS_FILE) == 0)
            found = 1;
    CHECK(rep && rep->verdict == LC_COMPILE_ERROR && found);
    CHECK_STR(rep && rep->build ? rep->build->prototype : NULL,
              "int Solution::add(int a, int b)");
    lc_report_free(rep);

    /* C++14 code needs --std. */
    rep = judge(ctx, "add", "cxx14.cpp", NULL, 1, NULL);
    CHECK(rep && rep->verdict == LC_COMPILE_ERROR);
    lc_report_free(rep);
    expect_accepted(ctx, "add", "cxx14.cpp", "c++14");
    expect_accepted(ctx, "add", "cxx17.cpp", "c++17");
    lc_close(ctx);
}

static void test_verdicts(void)
{
    static const struct {
        const char *src;
        lc_verdict v;
        size_t passed;
        const char *message;
    } want[] = {
        {"wrong.c", LC_WRONG_ANSWER, 1, NULL},
        {"crash.c", LC_RUNTIME_ERROR, 2, "killed by SIGSEGV"},
        {"exit1.c", LC_RUNTIME_ERROR, 2, "exited with status 1"},
        {"throw.cpp", LC_RUNTIME_ERROR, 2, "killed by SIGABRT"},
        {"spin.c", LC_TIME_LIMIT_EXCEEDED, 2, NULL},
        {"alloc.c", LC_MEMORY_LIMIT_EXCEEDED, 2, NULL},
        {"flood.c", LC_OUTPUT_LIMIT_EXCEEDED, 2,
         "printed more than 1 MiB on stdout"},
    };
    lc_ctx *ctx = open_ctx(JUDGE);
    const lc_problem *p = lc_problem_find(ctx, "add");
    size_t i;

    for (i = 0; i < sizeof want / sizeof want[0]; i++) {
        lc_judge_options opts;
        lc_report *rep = NULL;
        char path[256];

        snprintf(path, sizeof path, JUDGE "/0010-add/%s", want[i].src);
        memset(&opts, 0, sizeof opts);
        opts.all = 1;
        opts.stop_on_failure = 1;
        CHECK_INT(lc_judge(ctx, p, lc_lang_for_path(path), path, &opts, NULL,
                           &rep), LC_OK);
        if (!rep)
            continue;
        CHECK_INT(rep->verdict, want[i].v);
        CHECK_INT(rep->passed, want[i].passed);
        CHECK_INT(rep->nresults, want[i].passed + 1);
        if (want[i].message && rep->nresults)
            CHECK_STR(rep->results[rep->nresults - 1].message,
                      want[i].message);
        if (rep->verdict != want[i].v)
            fprintf(stderr, "    %s: %s\n", want[i].src,
                    lc_verdict_name(rep->verdict));
        lc_report_free(rep);
    }

    /* The C harness reports a size the solution didn't set. */
    {
        lc_report *rep = judge(ctx, "reverse", "unset.c", NULL, 1, NULL);

        CHECK(rep && rep->verdict == LC_RUNTIME_ERROR && rep->nresults);
        if (rep && rep->nresults) {
            CHECK_STR(rep->results[0].message, "exited with status 70");
            CHECK_STR(rep->results[0].stderr_text,
                      "lc: reverse did not set *returnSize\n");
        }
        lc_report_free(rep);
        rep = judge(ctx, "reverse", "null.c", NULL, 1, NULL);
        CHECK(rep && rep->nresults);
        if (rep && rep->nresults)
            CHECK_STR(rep->results[0].stderr_text,
                      "lc: reverse returned NULL, but *returnSize is 3\n");
        lc_report_free(rep);
    }

    /* check: a parameter after the call is the result, and for prefix the
     * harness makes sure the count fits it, in either language. */
    {
        static const struct {
            const char *pack, *src;
            lc_verdict v;
            const char *output, *err;
        } inplace[] = {
            {"merge", "wrong.c", LC_WRONG_ANSWER, "[1,3,5,0,0]", ""},
            {"dedup", "wrong.c", LC_WRONG_ANSWER, "[1,1,2]", ""},
            {"dedup", "big.c", LC_RUNTIME_ERROR, NULL,
             "lc: dedup returned 6, but nums has 5 elements\n"},
            {"dedup", "neg.cpp", LC_RUNTIME_ERROR, NULL,
             "lc: dedup returned -1, but nums has 5 elements\n"},
            {"dedup", "short.cpp", LC_RUNTIME_ERROR, NULL,
             "lc: dedup returned 5, but nums has 0 elements\n"},
            {"merge", "past.c", LC_RUNTIME_ERROR, NULL,
             "lc: merge wrote past the end of a (a[5])\n"},
            {"upper", "nullrow.c", LC_RUNTIME_ERROR, NULL,
             "lc: upper left grid[0] NULL, but its size is 2\n"},
        };

        for (i = 0; i < sizeof inplace / sizeof inplace[0]; i++) {
            lc_report *rep = judge(ctx, inplace[i].pack, inplace[i].src,
                                   NULL, 1, NULL);

            CHECK(rep && rep->nresults);
            if (!rep || !rep->nresults) {
                lc_report_free(rep);
                continue;
            }
            CHECK_INT(rep->verdict, inplace[i].v);
            CHECK_INT(rep->results[0].verdict, inplace[i].v);
            if (inplace[i].output)
                CHECK_STR(rep->results[0].output, inplace[i].output);
            else
                CHECK(rep->results[0].output == NULL);
            CHECK_STR(rep->results[0].stderr_text, inplace[i].err);
            lc_report_free(rep);
        }
    }
    lc_close(ctx);
}

static void expect_judge_error(lc_ctx *ctx, const char *problem,
                               const char *src, const char *std,
                               const char *cases, int rc, const char *msg)
{
    const lc_problem *p = lc_problem_find(ctx, problem);
    lc_judge_options opts;
    lc_cases *mine = NULL;
    lc_report *rep = (lc_report *)&opts;
    lc_hooks hooks;
    events ev;
    char path[256];

    if (!p)
        return;
    snprintf(path, sizeof path, "%s/%s", p->dir, src);
    memset(&opts, 0, sizeof opts);
    opts.examples = 1;
    opts.std = std;
    if (cases) {
        char cpath[256];

        snprintf(cpath, sizeof cpath, "%s/%s", p->dir, cases);
        CHECK_INT(lc_load_user_cases(ctx, cpath, &mine), LC_OK);
        opts.cases = mine;
    }
    memset(&hooks, 0, sizeof hooks);
    memset(&ev, 0, sizeof ev);
    hooks.on_event = on_event;
    hooks.ud = &ev;
    CHECK_INT(lc_judge(ctx, p, lc_lang_for_path(src), path, &opts, &hooks,
                       &rep), rc);
    CHECK(rep == NULL);
    if (msg && !strstr(lc_last_error(ctx), msg)) {
        CHECK(!"unexpected error message");
        fprintf(stderr, "    got: %s\n    expected it to contain: %s\n",
                lc_last_error(ctx), msg);
    }
    lc_cases_free(mine);
}

static void test_errors(void)
{
    lc_ctx *ctx = open_ctx(JUDGE);
    const lc_problem *p = lc_problem_find(ctx, "add");
    lc_report *rep = NULL;

    expect_judge_error(ctx, "add", "missing.c", NULL, NULL, LC_ERR_NOTFOUND,
                       "cannot open '");
    expect_judge_error(ctx, "tree", "x.c", NULL, NULL, LC_ERR_INVALID,
                       "problem 13 (tree): TreeNode parameters are not "
                       "supported yet");
    expect_judge_error(ctx, "unsigned", "x.c", NULL, NULL, LC_ERR_INVALID,
                       "problem 14 (unsigned) has no signature");
    expect_judge_error(ctx, "add", "solution.cpp", "c++98", NULL,
                       LC_ERR_INVALID, "unknown C++ standard 'c++98' "
                       "(expected c++11, c++14, c++17, c++20, c++23 or "
                       "max)");
    expect_judge_error(ctx, "add", "solution.c", "c++17", NULL,
                       LC_ERR_INVALID, "unknown C standard 'c++17' "
                       "(expected c99, c11, c17, c23 or max)");
    /* Cases are checked before anything is built. */
    expect_judge_error(ctx, "add", "solution.c", NULL, "badcase.txt",
                       LC_ERR_PARSE, "badcase.txt:1: b: column 1: expected "
                       "int, got a string");
    expect_judge_error(ctx, "reverse", "solution.c", NULL, "cases.txt",
                       LC_ERR_INVALID, "cases.txt:2: this case has no "
                       "expected result ('=> ...'), and problem 11 "
                       "(reverse) has no reference solution to work one "
                       "out");
    expect_judge_error(ctx, "bad-ref", "solution.c", NULL, "cases.txt",
                       LC_ERR_INVALID, "cases.txt:3: the reference solution "
                       "failed on this case (Runtime Error: killed by "
                       "SIGSEGV)");

    CHECK_INT(lc_judge(ctx, p, lc_lang_find("c"), JUDGE "/0010-add/ref.c",
                       NULL, NULL, NULL), LC_ERR_INVALID);
    CHECK_INT(lc_judge(ctx, NULL, lc_lang_find("c"), "x", NULL, NULL, &rep),
              LC_ERR_INVALID);
    CHECK(rep == NULL);
    lc_close(ctx);
}

static void test_cancel(void)
{
    lc_ctx *ctx = open_ctx(JUDGE);
    const lc_problem *p = lc_problem_find(ctx, "add");
    volatile sig_atomic_t flag = 1;
    lc_hooks hooks;
    lc_report *rep = NULL;
    events ev;

    memset(&hooks, 0, sizeof hooks);
    memset(&ev, 0, sizeof ev);
    hooks.on_event = on_event;
    hooks.ud = &ev;
    hooks.cancel = &flag;
    CHECK_INT(lc_judge(ctx, p, lc_lang_find("c"), JUDGE "/0010-add/ref.c",
                       NULL, &hooks, &rep), LC_OK);
    CHECK(rep && rep->cancelled && rep->verdict == LC_VERDICT_NONE);
    CHECK(rep && rep->nresults == 0);
    CHECK_STR(ev.log, "E");
    lc_report_free(rep);
    lc_close(ctx);
}

/* Standards: the default, $LC_CXXSTD, max, and a compiler without the
 * newest one (a script in front of the real compiler). */
static void test_std(void)
{
    const lc_lang *c = lc_lang_find("c"), *cpp = lc_lang_find("cpp");
    const char *real = getenv("CXX"), *std;
    lc_ctx *ctx = open_ctx(JUDGE);
    lc_str script = LC_STR_INIT;
    char *old = getenv("CXX") ? strdup(getenv("CXX")) : NULL;
    char fake[512];

    CHECK_INT(lc_std_resolve(ctx, cpp, NULL, &std), LC_OK);
    CHECK_STR(std, "c++11");
    CHECK_INT(lc_std_resolve(ctx, c, NULL, &std), LC_OK);
    CHECK_STR(std, "c11");
    CHECK_INT(lc_std_resolve(ctx, cpp, "c++17", &std), LC_OK);
    CHECK_STR(std, "c++17");
    CHECK_INT(lc_std_resolve(ctx, cpp, "max", &std), LC_OK);
    CHECK(std && strcmp(std, "c++17") >= 0);
    setenv("LC_CXXSTD", "c++14", 1);
    CHECK_INT(lc_std_resolve(ctx, cpp, NULL, &std), LC_OK);
    CHECK_STR(std, "c++14");
    CHECK_INT(lc_std_resolve(ctx, cpp, "c++11", &std), LC_OK);
    CHECK_STR(std, "c++11");
    setenv("LC_CXXSTD", "gnu++17", 1);
    CHECK_INT(lc_std_resolve(ctx, cpp, NULL, &std), LC_ERR_INVALID);
    CHECK_STR(lc_last_error(ctx), "unknown C++ standard 'gnu++17' in "
              "$LC_CXXSTD (expected c++11, c++14, c++17, c++20, c++23 or "
              "max)");
    unsetenv("LC_CXXSTD");

    /* A compiler that rejects c++23 and c++2b. */
    if (!getcwd(fake, sizeof fake - 64))
        fake[0] = '\0';
    strcat(fake, "/" TMP "/fake-c++");
    lc_str_printf(&script, "#!/bin/sh\nfor a do\n  case $a in\n"
                  "  -std=c++23|-std=c++2b) echo no >&2; exit 1;;\n"
                  "  esac\ndone\nexec %s \"$@\"\n",
                  real && *real ? real : "c++");
    write_file(fake, lc_str_cstr(&script));
    chmod(fake, 0755);
    setenv("CXX", fake, 1);
    CHECK_INT(lc_std_resolve(ctx, cpp, "c++23", &std), LC_ERR_INVALID);
    CHECK(strstr(lc_last_error(ctx), "does not support c++23") != NULL);
    CHECK_INT(lc_std_resolve(ctx, cpp, "max", &std), LC_OK);
    CHECK_STR(std, "c++20");
    /* The answers are cached: a second time needs no probe. */
    CHECK_INT(lc_std_resolve(ctx, cpp, "c++23", &std), LC_ERR_INVALID);
    {
        lc_str cache = LC_STR_INIT;

        CHECK_INT(lc_read_file(ctx, CACHE "/stds", LC_MIB, &cache), LC_OK);
        CHECK(strstr(lc_str_cstr(&cache), "fake-c++") != NULL);
        CHECK(strstr(lc_str_cstr(&cache), "\tc++23\t-\n") != NULL);
        lc_str_free(&cache);
    }
    setenv("CXX", "/no/such/compiler", 1);
    CHECK(!lc_lang_installed(ctx, cpp));
    CHECK_INT(lc_std_resolve(ctx, cpp, NULL, &std), LC_ERR_NOTFOUND);
    CHECK_STR(lc_last_error(ctx), "the C++ compiler '/no/such/compiler' "
              "(from $CXX) was not found");
    if (old)
        setenv("CXX", old, 1);
    else
        unsetenv("CXX");
    CHECK(lc_lang_installed(ctx, cpp));
    free(old);
    lc_str_free(&script);
    lc_close(ctx);
}

/* With the sanitizers, a read past the end of an array is a Runtime
 * Error. Skipped where the compiler has no sanitizers (musl). */
static void test_sanitize(void)
{
    lc_ctx *ctx = open_ctx(JUDGE);
    const lc_problem *p = lc_problem_find(ctx, "add");
    lc_judge_options opts;
    lc_report *rep = NULL;

    memset(&opts, 0, sizeof opts);
    opts.all = 1;
    opts.sanitize = 1;
    CHECK_INT(lc_judge(ctx, p, lc_lang_find("c"),
                       JUDGE "/0010-add/overflow.c", &opts, NULL, &rep),
              LC_OK);
    if (rep && rep->verdict == LC_COMPILE_ERROR) {
        printf("test_build: note: skipped the sanitizer test (the C "
               "compiler has no sanitizers)\n");
    } else if (rep) {
        char dsym[512] = "";

        CHECK(rep->build && rep->build->sanitize);
        CHECK_INT(rep->verdict, LC_RUNTIME_ERROR);
        CHECK_INT(rep->passed, 4);
        CHECK(rep->nresults == 5 &&
              rep->results[2].verdict == LC_RUNTIME_ERROR);
#ifdef __APPLE__
        /* The debug info dsymutil wrote is under the program's name,
         * where atos looks for it, and no copy is left under the name it
         * was built as. */
        CHECK_INT(entries(ADD_C "-sanitize", "prog-", ".dSYM", dsym,
                          sizeof dsym), 1);
        CHECK(strstr(dsym, ".tmp-") == NULL);
        if (strlen(dsym) > 5) {
            dsym[strlen(dsym) - 5] = '\0';
            CHECK(access(dsym, X_OK) == 0);
        }
#else
        CHECK_INT(entries(ADD_C "-sanitize", "prog-", ".dSYM", dsym,
                          sizeof dsym), 0);
#endif
    }
    lc_report_free(rep);
    lc_close(ctx);
}

int main(void)
{
    signal(SIGPIPE, SIG_IGN);
    if (system("rm -rf " TMP " && mkdir -p " TMP) != 0)
        return 1;
    RUN(test_types);
    RUN(test_stubs_compile);
    RUN(test_cache_and_events);
    RUN(test_cached_output);
    RUN(test_compile_error);
    RUN(test_verdicts);
    RUN(test_errors);
    RUN(test_cancel);
    RUN(test_std);
    RUN(test_sanitize);
    return test_report();
}
