/*
 * test_judge.c: verdict names, limits, lc_classify, and lc_judge_exe on
 * the Two Sum fixture pack with a hand-written harness
 * (tests/fixtures/run/twosum.c) that behaves as $LC_FIXTURE_MODE says.
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"
#include "test.h"

#define FIXTURES "tests/fixtures/problems"
#define TWOSUM "tests/fixtures/run/twosum"

static void test_names(void)
{
    CHECK_STR(lc_verdict_name(LC_ACCEPTED), "Accepted");
    CHECK_STR(lc_verdict_name(LC_WRONG_ANSWER), "Wrong Answer");
    CHECK_STR(lc_verdict_name(LC_COMPILE_ERROR), "Compile Error");
    CHECK_STR(lc_verdict_name(LC_RUNTIME_ERROR), "Runtime Error");
    CHECK_STR(lc_verdict_name(LC_TIME_LIMIT_EXCEEDED), "Time Limit Exceeded");
    CHECK_STR(lc_verdict_name(LC_MEMORY_LIMIT_EXCEEDED),
              "Memory Limit Exceeded");
    CHECK_STR(lc_verdict_name(LC_OUTPUT_LIMIT_EXCEEDED),
              "Output Limit Exceeded");
    CHECK_STR(lc_verdict_name(LC_VERDICT_NONE), "No Verdict");
    CHECK_STR(lc_verdict_name((lc_verdict)99), "No Verdict");
    CHECK_STR(lc_verdict_id(LC_ACCEPTED), "accepted");
    CHECK_STR(lc_verdict_id(LC_WRONG_ANSWER), "wrong_answer");
    CHECK_STR(lc_verdict_id(LC_COMPILE_ERROR), "compile_error");
    CHECK_STR(lc_verdict_id(LC_RUNTIME_ERROR), "runtime_error");
    CHECK_STR(lc_verdict_id(LC_TIME_LIMIT_EXCEEDED), "time_limit_exceeded");
    CHECK_STR(lc_verdict_id(LC_MEMORY_LIMIT_EXCEEDED),
              "memory_limit_exceeded");
    CHECK_STR(lc_verdict_id(LC_OUTPUT_LIMIT_EXCEEDED),
              "output_limit_exceeded");
    CHECK_STR(lc_verdict_id(LC_VERDICT_NONE), "none");
    CHECK_STR(lc_signal_name(SIGSEGV), "SIGSEGV");
    CHECK_STR(lc_signal_name(SIGABRT), "SIGABRT");
    CHECK_STR(lc_signal_name(SIGKILL), "SIGKILL");
    CHECK_STR(lc_signal_name(SIGFPE), "SIGFPE");
    CHECK(lc_signal_name(0) == NULL);
    CHECK(lc_signal_name(12345) == NULL);
}

static void test_limits(void)
{
    lc_limits l;
    lc_run_spec s;

    lc_limits_init(&l, 1000, 256);
    CHECK_INT(l.output_max, LC_MIB);
    CHECK_INT(l.result_max, 16 * LC_MIB);
    memset(&s, 0, sizeof s);
    lc_limits_apply(&l, &s);
    CHECK_INT(s.cpu_sec, 2);
    CHECK_INT(s.wall_ms, 3000);
    CHECK(s.as_bytes == 576ull * LC_MIB);
    CHECK(s.stack_bytes == 256ull * LC_MIB);
    CHECK(s.fsize_bytes == 16ull * LC_MIB);
    CHECK_INT(s.stdout_max, LC_MIB);
    CHECK_INT(s.stderr_max, LC_MIB);
    CHECK_INT(s.result_max, 16 * LC_MIB);

    lc_limits_init(&l, 1500, 64);
    lc_limits_apply(&l, &s);
    CHECK_INT(s.cpu_sec, 3);
    CHECK_INT(s.wall_ms, 4000);
    CHECK(s.as_bytes == 192ull * LC_MIB);

    /* No limits at all. */
    lc_limits_init(&l, 0, 0);
    lc_limits_apply(&l, &s);
    CHECK_INT(s.cpu_sec, 0);
    CHECK_INT(s.wall_ms, 0);
    CHECK(s.as_bytes == 0);
}

/* A run that exited 0 within every limit. */
static lc_run_result clean_run(void)
{
    lc_run_result r;

    lc_run_result_init(&r);
    r.exited = 1;
    r.cpu_ms = 10;
    r.wall_ms = 12;
    r.max_rss_kb = 2048;
    return r;
}

static void expect(const lc_limits *l, const lc_run_result *r,
                   lc_verdict want, const char *why)
{
    lc_str s = LC_STR_INIT;

    CHECK_INT(lc_classify(l, r, &s), want);
    CHECK_STR(lc_str_cstr(&s), why);
    lc_str_free(&s);
}

static void test_classify(void)
{
    lc_limits l;
    lc_run_result r;

    lc_limits_init(&l, 1000, 256);
    r = clean_run();
    expect(&l, &r, LC_VERDICT_NONE, "");

    r = clean_run();
    r.exit_code = 1;
    expect(&l, &r, LC_RUNTIME_ERROR, "exited with status 1");
    r = clean_run();
    r.exited = 0;
    r.signal = SIGSEGV;
    expect(&l, &r, LC_RUNTIME_ERROR, "killed by SIGSEGV");
    r.signal = 77;
    expect(&l, &r, LC_RUNTIME_ERROR, "killed by signal 77");

    r = clean_run();
    r.cpu_ms = 1001;
    expect(&l, &r, LC_TIME_LIMIT_EXCEEDED,
           "used 1001 ms of CPU time; the limit is 1000 ms");
    r.cpu_ms = 1000;
    expect(&l, &r, LC_VERDICT_NONE, "");
    r = clean_run();
    r.exited = 0;
    r.signal = SIGXCPU;
    r.cpu_ms = 990;
    expect(&l, &r, LC_TIME_LIMIT_EXCEEDED,
           "used 990 ms of CPU time; the limit is 1000 ms");
    r = clean_run();
    r.exited = 0;
    r.signal = SIGKILL;
    r.timed_out = 1;
    r.wall_ms = 3004;
    r.cpu_ms = 3;
    expect(&l, &r, LC_TIME_LIMIT_EXCEEDED,
           "was still running after 3004 ms (it used 3 ms of CPU time; the "
           "limit is 1000 ms)");

    r = clean_run();
    r.max_rss_kb = 256 * 1024 + 1;
    expect(&l, &r, LC_MEMORY_LIMIT_EXCEEDED,
           "used 257 MB of memory; the limit is 256 MB");
    r.max_rss_kb = 256 * 1024;
    expect(&l, &r, LC_VERDICT_NONE, "");
    r = clean_run();
    r.exited = 0;
    r.signal = SIGKILL;
    r.mem_killed = 1;
    expect(&l, &r, LC_MEMORY_LIMIT_EXCEEDED,
           "used over 576 MB of memory; the limit is 256 MB");

    r = clean_run();
    r.overflow = LC_RUN_STDOUT;
    expect(&l, &r, LC_OUTPUT_LIMIT_EXCEEDED,
           "printed more than 1 MiB on stdout");
    r.overflow = LC_RUN_STDERR;
    expect(&l, &r, LC_OUTPUT_LIMIT_EXCEEDED,
           "printed more than 1 MiB on stderr");
    r.overflow = LC_RUN_RESULT;
    expect(&l, &r, LC_OUTPUT_LIMIT_EXCEEDED,
           "returned a result longer than 16 MiB");

    /* Precedence: output, then memory, then time, then the exit. */
    r = clean_run();
    r.exited = 0;
    r.signal = SIGKILL;
    r.timed_out = 1;
    r.cpu_ms = 5000;
    r.max_rss_kb = 1024 * 1024;
    r.overflow = LC_RUN_STDOUT;
    CHECK_INT(lc_classify(&l, &r, &r.out), LC_OUTPUT_LIMIT_EXCEEDED);
    r.overflow = 0;
    CHECK_INT(lc_classify(&l, &r, &r.out), LC_MEMORY_LIMIT_EXCEEDED);
    r.max_rss_kb = 1024;
    CHECK_INT(lc_classify(&l, &r, &r.out), LC_TIME_LIMIT_EXCEEDED);
    lc_str_free(&r.out);

    /* No limits: only the exit matters. */
    lc_limits_init(&l, 0, 0);
    r = clean_run();
    r.cpu_ms = 99999;
    r.max_rss_kb = 99999999;
    expect(&l, &r, LC_VERDICT_NONE, "");
}

/* ------------------------------------------------------------------ */
/* Judging the Two Sum fixture                                         */
/* ------------------------------------------------------------------ */

typedef struct events {
    char log[256];            /* one letter per event: S, D or E */
    size_t starts, dones;
    size_t cancel_after;      /* set the cancel flag after this many done */
    volatile sig_atomic_t cancel;
    int bad;                  /* an event was out of order */
} events;

static void on_event(void *ud, const lc_event *ev)
{
    events *e = ud;
    size_t n = strlen(e->log);

    if (n + 1 < sizeof e->log)
        e->log[n] = ev->kind == LC_EV_CASE_START  ? 'S'
                    : ev->kind == LC_EV_CASE_DONE ? 'D'
                                                  : 'E';
    switch (ev->kind) {
    case LC_EV_CASE_START:
        if (ev->index != e->starts || !ev->test_case || ev->result)
            e->bad = 1;
        e->starts++;
        break;
    case LC_EV_CASE_DONE:
        if (ev->index != e->dones || !ev->result ||
            ev->result->index != ev->index)
            e->bad = 1;
        e->dones++;
        if (e->cancel_after && e->dones == e->cancel_after)
            e->cancel = 1;
        break;
    case LC_EV_DONE:
        if (!ev->report)
            e->bad = 1;
        break;
    case LC_EV_BUILD_START:
    case LC_EV_BUILD_DONE:
    case LC_EV_FINDING:
        e->bad = 1; /* lc_judge_exe builds and checks nothing */
        break;
    }
}

static lc_ctx *ctx;
static const lc_problem *two_sum;
static lc_cases *cases;

/* Judges the fixture in `mode`. Returns the report, or NULL after a
 * failed check. */
static lc_report *judge(const char *mode, const lc_judge_opts *opts,
                        events *ev)
{
    lc_hooks hooks;
    lc_report *r = NULL;
    int rc;

    setenv("LC_FIXTURE_MODE", mode, 1);
    memset(&hooks, 0, sizeof hooks);
    hooks.on_event = on_event;
    hooks.ud = ev;
    hooks.cancel = &ev->cancel;
    rc = lc_judge_exe(ctx, two_sum, TWOSUM, cases->items, cases->count, opts,
                      &hooks, &r);
    CHECK_INT(rc, LC_OK);
    if (rc != LC_OK)
        fprintf(stderr, "    [%s] error: %s\n", mode, lc_last_error(ctx));
    CHECK_INT(ev->bad, 0);
    return r;
}

static void test_accepted(void)
{
    events ev;
    lc_report *r;
    const lc_case_result *c;

    memset(&ev, 0, sizeof ev);
    r = judge("correct", NULL, &ev);
    if (!r)
        return;
    CHECK_INT(r->verdict, LC_ACCEPTED);
    CHECK_INT(r->cancelled, 0);
    CHECK_INT(r->total, 5);
    CHECK_INT(r->nresults, 5);
    CHECK_INT(r->passed, 5);
    CHECK_STR(ev.log, "SDSDSDSDSDE");
    CHECK(r->max_memory_kb > 0);

    c = &r->results[0];
    CHECK_INT(c->index, 0);
    CHECK_INT(c->verdict, LC_ACCEPTED);
    CHECK(c->message == NULL);
    CHECK_INT(c->example, 1);
    CHECK_INT(c->line, 3);
    CHECK_INT(c->ninputs, 2);
    if (c->ninputs == 2) {
        CHECK_STR(c->inputs[0], "[6,3,1,8]");
        CHECK_STR(c->inputs[1], "11");
    }
    CHECK_STR(c->expected, "[1,3]");
    CHECK_STR(c->output, "[1,3]");
    CHECK_INT(c->stdout_len, 0);
    CHECK_STR(c->stdout_text, "");
    CHECK_INT(c->stderr_len, 0);
    CHECK_INT(c->exit_code, 0);
    CHECK_INT(c->signal, 0);
    CHECK(c->memory_kb > 0);

    c = &r->results[4];
    CHECK_INT(c->example, 0);
    CHECK_STR(c->inputs[0], "[-1000000,999999,1]");
    CHECK_STR(c->output, "[0,2]");
    lc_report_free(r);

    /* compare: unordered 0 takes the pair in either order. */
    memset(&ev, 0, sizeof ev);
    r = judge("reversed", NULL, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_ACCEPTED);
        CHECK_STR(r->results[0].output, "[3,1]");
        lc_report_free(r);
    }
}

static void test_wrong_answer(void)
{
    lc_judge_opts opts;
    events ev;
    lc_report *r;

    memset(&ev, 0, sizeof ev);
    r = judge("wrong", NULL, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_WRONG_ANSWER);
        CHECK_INT(r->nresults, 5);
        CHECK_INT(r->passed, 0);
        CHECK_INT(r->results[0].verdict, LC_WRONG_ANSWER);
        CHECK(r->results[0].message == NULL);
        CHECK_STR(r->results[0].output, "[0,0]");
        CHECK_STR(r->results[0].expected, "[1,3]");
        lc_report_free(r);
    }

    /* Stopping at the first failure, as submitting does. */
    memset(&opts, 0, sizeof opts);
    opts.stop_on_failure = 1;
    memset(&ev, 0, sizeof ev);
    r = judge("wrong", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_WRONG_ANSWER);
        CHECK_INT(r->total, 5);
        CHECK_INT(r->nresults, 1);
        CHECK_STR(ev.log, "SDE");
        lc_report_free(r);
    }
}

static void test_runtime_errors(void)
{
    lc_judge_opts opts;
    events ev;
    lc_report *r;

    /* Cases 2 and 5 have negative numbers. */
    memset(&ev, 0, sizeof ev);
    r = judge("crash-negative", NULL, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_RUNTIME_ERROR);
        CHECK_INT(r->nresults, 5);
        CHECK_INT(r->passed, 3);
        CHECK_INT(r->results[0].verdict, LC_ACCEPTED);
        CHECK_INT(r->results[1].verdict, LC_RUNTIME_ERROR);
        CHECK_STR(r->results[1].message, "killed by SIGSEGV");
        CHECK_INT(r->results[1].signal, SIGSEGV);
        CHECK_INT(r->results[1].exit_code, -1);
        CHECK(r->results[1].output == NULL);
        CHECK_STR(r->results[1].expected, "[0,1]");
        CHECK_INT(r->results[4].verdict, LC_RUNTIME_ERROR);
        lc_report_free(r);
    }
    memset(&opts, 0, sizeof opts);
    opts.stop_on_failure = 1;
    memset(&ev, 0, sizeof ev);
    r = judge("crash-negative", &opts, &ev);
    if (r) {
        CHECK_INT(r->nresults, 2);
        CHECK_INT(r->passed, 1);
        lc_report_free(r);
    }

    memset(&ev, 0, sizeof ev);
    r = judge("silent", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_RUNTIME_ERROR);
        CHECK_STR(r->results[0].message, "exited without returning a result");
        lc_report_free(r);
    }
    memset(&ev, 0, sizeof ev);
    r = judge("garbage", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_RUNTIME_ERROR);
        CHECK_STR(r->results[0].message,
                  "its result could not be read: byte 5: expected an int, "
                  "got 'x'");
        CHECK(r->results[0].output == NULL);
        lc_report_free(r);
    }
    memset(&ev, 0, sizeof ev);
    r = judge("extra", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_RUNTIME_ERROR);
        CHECK_STR(r->results[0].message,
                  "its result has unexpected text at byte 7");
        lc_report_free(r);
    }
    /* A right answer doesn't excuse a failed exit. */
    memset(&ev, 0, sizeof ev);
    r = judge("exit1", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_RUNTIME_ERROR);
        CHECK_STR(r->results[0].message, "exited with status 1");
        CHECK_INT(r->results[0].exit_code, 1);
        lc_report_free(r);
    }
}

static void test_limits_exceeded(void)
{
    lc_judge_opts opts;
    events ev;
    lc_report *r;
    const char *m;

    memset(&opts, 0, sizeof opts);
    opts.stop_on_failure = 1;
    opts.time_ms = 100;
    memset(&ev, 0, sizeof ev);
    r = judge("spin", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_TIME_LIMIT_EXCEEDED);
        CHECK_INT(r->nresults, 1);
        m = r->results[0].message;
        CHECK(m && (strncmp(m, "used ", 5) == 0 ||
                    strncmp(m, "was still running after ", 24) == 0));
        CHECK(r->results[0].time_ms > 100);
        lc_report_free(r);
    }

    /* The plan's "allocates 2 GB". */
    opts.time_ms = 0;
    opts.memory_mb = 64;
    memset(&ev, 0, sizeof ev);
    r = judge("alloc", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_MEMORY_LIMIT_EXCEEDED);
        m = r->results[0].message;
        CHECK(m && strncmp(m, "used ", 5) == 0 &&
              strstr(m, "the limit is 64 MB") != NULL);
        CHECK(r->results[0].memory_kb > 64 * 1024);
        lc_report_free(r);
    }

    opts.memory_mb = 0;
    memset(&ev, 0, sizeof ev);
    r = judge("flood", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_OUTPUT_LIMIT_EXCEEDED);
        CHECK_STR(r->results[0].message, "printed more than 1 MiB on stdout");
        CHECK_INT(r->results[0].stdout_len, LC_REPORT_TEXT_MAX);
        CHECK_INT(r->results[0].stdout_truncated, 1);
        lc_report_free(r);
    }
}

static void test_output_kept(void)
{
    lc_judge_opts opts;
    events ev;
    lc_report *r;
    const lc_case_result *c;

    memset(&opts, 0, sizeof opts);
    memset(&ev, 0, sizeof ev);
    r = judge("chatty", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_ACCEPTED);
        c = &r->results[0];
        CHECK_STR(c->stdout_text, "n = 4\n");
        CHECK_INT(c->stdout_len, 6);
        CHECK_INT(c->stdout_truncated, 0);
        CHECK_STR(c->stderr_text, "target = 11\n");
        CHECK_INT(c->stderr_truncated, 0);
        lc_report_free(r);
    }

    /* 100 KB on each: stdout keeps its head, stderr its tail from the
     * start of a line. */
    memset(&ev, 0, sizeof ev);
    r = judge("verbose", &opts, &ev);
    if (r) {
        CHECK_INT(r->verdict, LC_ACCEPTED);
        c = &r->results[0];
        CHECK_INT(c->stdout_len, LC_REPORT_TEXT_MAX);
        CHECK_INT(c->stdout_truncated, 1);
        CHECK(strncmp(c->stdout_text, "stdout line   0\n", 16) == 0);
        CHECK_INT(c->stderr_truncated, 1);
        CHECK(c->stderr_len <= LC_REPORT_TEXT_MAX);
        CHECK(c->stderr_len > LC_REPORT_TEXT_MAX - 16);
        CHECK(c->stderr_len % 16 == 0);
        CHECK(strncmp(c->stderr_text, "stderr line ", 12) == 0);
        CHECK(c->stderr_len >= 16 &&
              strcmp(c->stderr_text + c->stderr_len - 16,
                     "stderr line 399\n") == 0);
        lc_report_free(r);
    }
}

static void test_cancel(void)
{
    events ev;
    lc_report *r;

    /* Cancelled from the callback after two cases. */
    memset(&ev, 0, sizeof ev);
    ev.cancel_after = 2;
    r = judge("correct", NULL, &ev);
    if (r) {
        CHECK_INT(r->cancelled, 1);
        CHECK_INT(r->verdict, LC_VERDICT_NONE);
        CHECK_INT(r->nresults, 2);
        CHECK_INT(r->passed, 2);
        CHECK_STR(ev.log, "SDSDE");
        lc_report_free(r);
    }

    /* After a failure, the failure's verdict stands. */
    memset(&ev, 0, sizeof ev);
    ev.cancel_after = 3;
    r = judge("crash-negative", NULL, &ev);
    if (r) {
        CHECK_INT(r->cancelled, 1);
        CHECK_INT(r->verdict, LC_RUNTIME_ERROR);
        CHECK_INT(r->nresults, 3);
        lc_report_free(r);
    }

    /* Cancelled before anything ran. */
    memset(&ev, 0, sizeof ev);
    ev.cancel = 1;
    r = judge("correct", NULL, &ev);
    if (r) {
        CHECK_INT(r->cancelled, 1);
        CHECK_INT(r->nresults, 0);
        CHECK_STR(ev.log, "E");
        lc_report_free(r);
    }
}

static void test_errors(void)
{
    lc_problem p;
    lc_report *r = NULL;
    lc_param hidden;
    lc_case bad;
    const char *args[1];

    /* Without hooks or options. */
    setenv("LC_FIXTURE_MODE", "correct", 1);
    CHECK_INT(lc_judge_exe(ctx, two_sum, TWOSUM, cases->items, cases->count,
                           NULL, NULL, &r), LC_OK);
    CHECK(r != NULL && r->verdict == LC_ACCEPTED);
    lc_report_free(r);
    r = NULL;

    CHECK_INT(lc_judge_exe(ctx, two_sum, "tests/tmp/judge-missing",
                           cases->items, cases->count, NULL, NULL, &r),
              LC_ERR_NOTFOUND);
    CHECK(r == NULL);
    CHECK_INT(lc_judge_exe(ctx, two_sum, TWOSUM, cases->items, 0, NULL, NULL,
                           &r), LC_ERR_INVALID);
    CHECK_STR(lc_last_error(ctx), "problem 1 (two-sum) has no test cases to "
              "run");

    p = *two_sum;
    p.sig = NULL;
    CHECK_INT(lc_judge_exe(ctx, &p, TWOSUM, cases->items, cases->count, NULL,
                           NULL, &r), LC_ERR_INVALID);
    CHECK_STR(lc_last_error(ctx), "problem 1 (two-sum) has no signature, so "
              "it can't be judged");
    p = *two_sum;
    p.compare.mode = LC_CMP_CHECKER;
    CHECK_INT(lc_judge_exe(ctx, &p, TWOSUM, cases->items, cases->count, NULL,
                           NULL, &r), LC_ERR_INVALID);
    CHECK_STR(lc_last_error(ctx), "problem 1 (two-sum): compare: checker is "
              "not supported yet");

    /* A case that doesn't fit the signature stops the run with its
     * error. */
    p = *two_sum;
    hidden.type.kind = LC_T_INT;
    hidden.type.dims = 0;
    hidden.name = "extra";
    p.hidden = &hidden;
    p.nhidden = 1;
    CHECK_INT(lc_judge_exe(ctx, &p, TWOSUM, cases->items, cases->count, NULL,
                           NULL, &r), LC_ERR_PARSE);
    CHECK(r == NULL);
    bad = cases->items[0];
    args[0] = "[1,2]";
    bad.args = args;
    bad.nargs = 1;
    CHECK_INT(lc_judge_exe(ctx, two_sum, TWOSUM, &bad, 1, NULL, NULL, &r),
              LC_ERR_PARSE);
    CHECK(r == NULL);

    CHECK_INT(lc_judge_exe(ctx, two_sum, NULL, cases->items, cases->count,
                           NULL, NULL, &r), LC_ERR_INVALID);
    lc_report_free(NULL);
}

int main(void)
{
    lc_options opt;

    signal(SIGPIPE, SIG_IGN); /* as every front-end must */
    RUN(test_names);
    RUN(test_limits);
    RUN(test_classify);

    lc_options_init(&opt);
    opt.problems_dir = FIXTURES;
    ctx = lc_open(&opt);
    CHECK(ctx != NULL);
    CHECK_INT(lc_load(ctx), LC_OK);
    two_sum = lc_problem_find(ctx, "1");
    CHECK(two_sum != NULL);
    if (two_sum && lc_load_cases(ctx, two_sum, &cases) == LC_OK &&
        cases->count == 5) {
        RUN(test_accepted);
        RUN(test_wrong_answer);
        RUN(test_runtime_errors);
        RUN(test_limits_exceeded);
        RUN(test_output_kept);
        RUN(test_cancel);
        RUN(test_errors);
    } else {
        CHECK(!"cannot load the Two Sum fixture's cases");
    }
    lc_cases_free(cases);
    lc_close(ctx);
    return test_report();
}
