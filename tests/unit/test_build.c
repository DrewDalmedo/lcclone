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
 * the reference), s case start, d case done, E done; for the sanitizer
 * check k build start, K build done, x case start, y case done. */
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
        ch = ev->sanitizer_check ? 'k' : ev->build->reference ? 'r' : 'b';
        CHECK_INT(ev->build->sanitizer_check, ev->sanitizer_check);
        break;
    case LC_EV_BUILD_DONE:
        ch = ev->sanitizer_check ? 'K' : ev->build->reference ? 'R' : 'B';
        CHECK_INT(ev->build->sanitizer_check, ev->sanitizer_check);
        e->cached = ev->build->cached;
        e->ok = ev->build->ok;
        break;
    case LC_EV_CASE_START:
        ch = ev->sanitizer_check ? 'x' : 's';
        break;
    case LC_EV_CASE_DONE:
        ch = ev->sanitizer_check ? 'y' : 'd';
        CHECK_INT(ev->result->sanitizer_check, ev->sanitizer_check);
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

/* Judges `src` of `problem` with `opts`, logging events in *ev (and, with
 * `flag`, setting it when the sanitizer check's build starts). */
typedef struct check_events {
    events ev;
    volatile sig_atomic_t *flag;
} check_events;

static void on_check_event(void *ud, const lc_event *e)
{
    check_events *ce = ud;

    on_event(&ce->ev, e);
    if (ce->flag && e->kind == LC_EV_BUILD_START && e->sanitizer_check)
        *ce->flag = 1;
}

static lc_report *judge_with(lc_ctx *ctx, const char *problem,
                             const char *src, const lc_judge_options *opts,
                             events *ev, volatile sig_atomic_t *flag)
{
    const lc_problem *p = lc_problem_find(ctx, problem);
    check_events ce;
    lc_hooks hooks;
    lc_report *rep = NULL;
    char path[256];

    CHECK(p != NULL);
    if (!p)
        return NULL;
    snprintf(path, sizeof path, "%s/%s", p->dir, src);
    memset(&ce, 0, sizeof ce);
    ce.flag = flag;
    memset(&hooks, 0, sizeof hooks);
    hooks.on_event = on_check_event;
    hooks.ud = &ce;
    hooks.cancel = flag;
    if (lc_judge(ctx, p, lc_lang_for_path(src), path, opts, &hooks, &rep) !=
        LC_OK) {
        fprintf(stderr, "    %s/%s: %s\n", problem, src, lc_last_error(ctx));
        return NULL;
    }
    *ev = ce.ev;
    return rep;
}

/* 1 if the case's stderr has `what`. */
static int stderr_has(const lc_case_result *r, const char *what)
{
    return r->stderr_text && strstr(r->stderr_text, what) != NULL;
}

/*
 * The sanitizer check (lc submit): an accepted run is run again with the
 * sanitizers, and a case that fails there decides the verdict. Fixture 19
 * reads one past the end of its array, which only ASan sees, and only
 * because arrays have no spare element under it. Skipped where the
 * compiler has no sanitizers (musl).
 */
static void test_sanitizer_check(void)
{
    static const char *const past[] = {"past.c", "past.cpp"};
    lc_ctx *ctx = open_ctx(JUDGE);
    volatile sig_atomic_t flag = 0;
    lc_judge_options opts;
    lc_report *rep;
    events ev;
    size_t i;

    memset(&opts, 0, sizeof opts);
    opts.all = 1;
    opts.stop_on_failure = 1;
    opts.sanitizer_check = 1;

    /* A right solution passes both runs; the results are the first's. */
    rep = judge_with(ctx, "missing", "solution.c", &opts, &ev, NULL);
    CHECK(rep != NULL);
    if (rep && rep->sanitizer_check == LC_SANCHECK_SKIPPED) {
        printf("test_build: note: skipped the sanitizer check tests (%s)\n",
               rep->sanitizer_check_note);
        lc_report_free(rep);
        lc_close(ctx);
        return;
    }
    if (rep) {
        CHECK_INT(rep->verdict, LC_ACCEPTED);
        CHECK_INT(rep->sanitizer_check, LC_SANCHECK_PASSED);
        CHECK(rep->sanitizer_check_note == NULL);
        CHECK(rep->build && !rep->build->sanitize &&
              !rep->build->sanitizer_check);
        CHECK(rep->check_build && rep->check_build->ok &&
              rep->check_build->sanitize && rep->check_build->sanitizer_check);
        CHECK_INT(rep->passed, 5);
        CHECK_INT(rep->nresults, 5);
        for (i = 0; i < rep->nresults; i++)
            CHECK(!rep->results[i].sanitizer_check);
        CHECK_STR(ev.log, "bBsdsdsdsdsdkKxyxyxyxyxyE");
    }
    lc_report_free(rep);

    /* A read past the end: the first run accepts it, the check doesn't,
     * and the run stops at the case that failed there. */
    for (i = 0; i < sizeof past / sizeof past[0]; i++) {
        rep = judge_with(ctx, "missing", past[i], &opts, &ev, NULL);
        CHECK(rep != NULL);
        if (!rep)
            continue;
        CHECK_INT(rep->verdict, LC_RUNTIME_ERROR);
        CHECK_INT(rep->sanitizer_check, LC_SANCHECK_FAILED);
        CHECK_INT(rep->total, 5);
        CHECK_INT(rep->passed, 1);
        CHECK_INT(rep->nresults, 2);
        if (rep->nresults == 2) {
            CHECK(!rep->results[0].sanitizer_check);
            CHECK(rep->results[1].sanitizer_check);
            CHECK_INT(rep->results[1].verdict, LC_RUNTIME_ERROR);
            if (i == 0)
                CHECK(stderr_has(&rep->results[1], "heap-buffer-overflow"));
        }
        CHECK_STR(ev.log, "bBsdsdsdsdsdkKxyxyE");
        lc_report_free(rep);
    }

    /* Without stop_on_failure, each case that failed in the check takes
     * its place among the first run's results. */
    opts.stop_on_failure = 0;
    rep = judge_with(ctx, "missing", "past.c", &opts, &ev, NULL);
    CHECK(rep != NULL);
    if (rep) {
        CHECK_INT(rep->verdict, LC_RUNTIME_ERROR);
        CHECK_INT(rep->nresults, 5);
        CHECK_INT(rep->passed, 3);
        for (i = 0; i < rep->nresults; i++)
            CHECK_INT(rep->results[i].sanitizer_check, i == 1 || i == 3);
        CHECK_STR(ev.log, "bBsdsdsdsdsdkKxyxyxyxyxyE");
    }
    lc_report_free(rep);
    opts.stop_on_failure = 1;

    /* Running out of time in the check doesn't count: the sanitizers
     * slow programs down, and the first run judged the time. */
    rep = judge_with(ctx, "missing", "slowsan.c", &opts, &ev, NULL);
    CHECK(rep != NULL);
    if (rep) {
        CHECK_INT(rep->verdict, LC_ACCEPTED);
        CHECK_INT(rep->sanitizer_check, LC_SANCHECK_PASSED);
        CHECK_INT(rep->passed, 5);
        CHECK(rep->nresults == 5 && !rep->results[4].sanitizer_check);
        CHECK_STR(ev.log, "bBsdsdsdsdsdkKxyxyxyxyxyE");
    }
    lc_report_free(rep);

    /* Without the check, the read past the end goes unseen. */
    opts.sanitizer_check = 0;
    rep = judge_with(ctx, "missing", "past.c", &opts, &ev, NULL);
    CHECK(rep && rep->verdict == LC_ACCEPTED &&
          rep->sanitizer_check == LC_SANCHECK_NONE && !rep->check_build);
    lc_report_free(rep);
    opts.sanitizer_check = 1;

    /* Built with the sanitizers already, there is no check, and ASan
     * reports the read itself. So it does a write past the end: under
     * ASan the harness gives arrays no spare element to catch it with. */
    opts.sanitize = 1;
    rep = judge_with(ctx, "missing", "past.c", &opts, &ev, NULL);
    CHECK(rep != NULL);
    if (rep) {
        CHECK_INT(rep->verdict, LC_RUNTIME_ERROR);
        CHECK_INT(rep->sanitizer_check, LC_SANCHECK_NONE);
        CHECK(rep->check_build == NULL);
        CHECK(rep->nresults == 2 && !rep->results[1].sanitizer_check &&
              stderr_has(&rep->results[1], "heap-buffer-overflow"));
        CHECK_STR(ev.log, "bBsdsdE");
    }
    lc_report_free(rep);
    rep = judge_with(ctx, "merge", "past.c", &opts, &ev, NULL);
    CHECK(rep != NULL);
    if (rep) {
        CHECK_INT(rep->verdict, LC_RUNTIME_ERROR);
        CHECK(rep->nresults >= 1 &&
              stderr_has(&rep->results[rep->nresults - 1],
                         "heap-buffer-overflow") &&
              stderr_has(&rep->results[rep->nresults - 1], "WRITE"));
    }
    lc_report_free(rep);
    opts.sanitize = 0;

    /* ^C during the check cancels the run, accepted or not. */
    rep = judge_with(ctx, "missing", "solution.c", &opts, &ev, &flag);
    CHECK(rep != NULL);
    if (rep) {
        CHECK(rep->cancelled);
        CHECK_INT(rep->verdict, LC_VERDICT_NONE);
        CHECK_INT(rep->sanitizer_check, LC_SANCHECK_CANCELLED);
        CHECK(ev.log[strlen(ev.log) - 1] == 'E');
        CHECK(strncmp(ev.log, "bBsdsdsdsdsdk", 13) == 0);
    }
    lc_report_free(rep);
    lc_close(ctx);
}

/* Writes an executable script at TMP/<name> and returns its absolute
 * path in `path`. */
static void fake_compiler(const char *name, const char *body, char *path,
                          size_t size)
{
    if (!getcwd(path, size - 64))
        path[0] = '\0';
    strcat(path, "/" TMP "/");
    strcat(path, name);
    write_file(path, body);
    chmod(path, 0755);
}

/*
 * The check is skipped, with a note saying why, when the compiler has no
 * sanitizers, when programs built with them don't run, and when the
 * solution doesn't build with them: scripts in front of the real C
 * compiler. What the probe found is kept in the cache.
 */
static void test_sanitizer_skip(void)
{
    static const struct {
        const char *name, *body, *note;
    } fakes[] = {
        {"nosan-cc",
         "#!/bin/sh\nfor a do\n  case $a in\n"
         "  -fsanitize=*) echo 'unknown option' >&2; exit 1;;\n"
         "  esac\ndone\nexec %s \"$@\"\n",
         " has none"},
        {"norun-cc",
         "#!/bin/sh\nsan= out= prev=\nfor a do\n"
         "  case $a in -fsanitize=*) san=1;; esac\n"
         "  [ \"$prev\" = -o ] && out=$a\n  prev=$a\ndone\n"
         "if [ -n \"$san\" ]; then\n"
         "  printf '#!/bin/sh\\nexit 3\\n' >\"$out\" && chmod +x \"$out\"\n"
         "  exit\nfi\nexec %s \"$@\"\n",
         "programs built with them don't run here (exited with status 3)"},
        {"nobuild-cc",
         "#!/bin/sh\nsan= probe=\nfor a do\n  case $a in\n"
         "  -fsanitize=*) san=1;;\n  *probe-san*) probe=1;;\n  esac\n"
         "done\nif [ -n \"$san\" ] && [ -z \"$probe\" ]; then\n"
         "  echo 'no sanitizers for you' >&2; exit 1\nfi\n"
         "exec %s \"$@\"\n",
         "it doesn't build with them"},
    };
    const char *real = getenv("CC");
    char *old = real ? strdup(real) : NULL;
    lc_ctx *ctx = open_ctx(JUDGE);
    lc_judge_options opts;
    lc_str body = LC_STR_INIT, cache = LC_STR_INIT;
    char fake[512];
    events ev;
    size_t i;

    memset(&opts, 0, sizeof opts);
    opts.all = 1;
    opts.stop_on_failure = 1;
    opts.sanitizer_check = 1;
    for (i = 0; i < sizeof fakes / sizeof fakes[0]; i++) {
        lc_report *rep;

        lc_str_clear(&body);
        lc_str_printf(&body, fakes[i].body, old && *old ? old : "cc");
        fake_compiler(fakes[i].name, lc_str_cstr(&body), fake, sizeof fake);
        setenv("CC", fake, 1);
        rep = judge_with(ctx, "missing", "past.c", &opts, &ev, NULL);
        CHECK(rep != NULL);
        if (i == 2 && rep && rep->sanitizer_check_note &&
            !strstr(rep->sanitizer_check_note, fakes[i].note)) {
            /* The real compiler's sanitizers don't work here either. */
            printf("test_build: note: skipped the 'doesn't build' test "
                   "(%s)\n", rep->sanitizer_check_note);
            lc_report_free(rep);
            continue;
        }
        if (rep) {
            CHECK_INT(rep->verdict, LC_ACCEPTED);
            CHECK_INT(rep->sanitizer_check, LC_SANCHECK_SKIPPED);
            CHECK(rep->sanitizer_check_note &&
                  strstr(rep->sanitizer_check_note, fakes[i].note));
            if (i < 2) {
                CHECK(rep->check_build == NULL);
                CHECK_STR(ev.log, "bBsdsdsdsdsdE");
            } else {
                CHECK(rep->check_build && !rep->check_build->ok);
                CHECK_STR(ev.log, "bBsdsdsdsdsdkKE");
            }
        }
        lc_report_free(rep);
    }
    /* The probe's answers are kept; the last compiler's is a yes. */
    CHECK_INT(lc_read_file(ctx, CACHE "/sanitizers", LC_MIB, &cache), LC_OK);
    CHECK(strstr(lc_str_cstr(&cache), "nosan-cc") != NULL);
    CHECK(strstr(lc_str_cstr(&cache), "\t-\tprograms built with them don't "
                 "run here (exited with status 3)\n") != NULL);
    CHECK(strstr(lc_str_cstr(&cache), "nobuild-cc") != NULL);
    if (old)
        setenv("CC", old, 1);
    else
        unsetenv("CC");
    free(old);
    lc_str_free(&body);
    lc_str_free(&cache);
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
    RUN(test_sanitizer_check);
    RUN(test_sanitizer_skip);
    return test_report();
}
