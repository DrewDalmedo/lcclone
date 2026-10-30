/*
 * test_runner.c: lc_run against programs that misbehave in one way each
 * (tests/fixtures/run/misbehave.c), as listed in docs/PLAN.md section 8.
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "internal.h"
#include "test.h"

#define TMP "tests/tmp/run"
#define MISBEHAVE "tests/fixtures/run/misbehave"
#define THROW "tests/fixtures/run/throw"

static lc_ctx *ctx;
static lc_run_result res;

static void nap_ms(long ms)
{
    struct timespec ts;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    while (nanosleep(&ts, &ts) != 0 && errno == EINTR)
        ;
}

/* A spec for misbehave with these arguments (after the program name), a
 * result pipe and a generous deadline. `argv` needs room for 6 entries. */
static void spec_for(lc_run_spec *s, char **argv, const char *a1,
                     const char *a2, const char *a3)
{
    memset(s, 0, sizeof *s);
    argv[0] = MISBEHAVE;
    argv[1] = (char *)a1;
    argv[2] = (char *)a2;
    argv[3] = a2 ? (char *)a3 : NULL;
    argv[4] = NULL;
    s->path = MISBEHAVE;
    s->argv = argv;
    s->result_fd = 1;
    s->wall_ms = 10000;
}

/* Runs and checks that lc_run itself succeeded. */
static int run_ok(const lc_run_spec *s)
{
    int rc = lc_run(ctx, s, &res);

    CHECK_INT(rc, LC_OK);
    if (rc != LC_OK)
        fprintf(stderr, "    error: %s\n", lc_last_error(ctx));
    return rc == LC_OK;
}

static int starts_with(const char *s, const char *prefix)
{
    int ok = strncmp(s, prefix, strlen(prefix)) == 0;

    if (!ok)
        fprintf(stderr, "    got:      %s\n    expected: %s...\n", s, prefix);
    return ok;
}

static int count_open_fds(void)
{
    int fd, n = 0;

    for (fd = 0; fd < 1024; fd++)
        if (fcntl(fd, F_GETFD) != -1)
            n++;
    return n;
}

static void test_ok(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "ok", NULL, NULL);
    if (!run_ok(&s))
        return;
    CHECK_INT(res.exited, 1);
    CHECK_INT(res.exit_code, 0);
    CHECK_INT(res.signal, 0);
    CHECK_INT(res.timed_out, 0);
    CHECK_INT(res.cancelled, 0);
    CHECK_INT(res.mem_killed, 0);
    CHECK_INT(res.overflow, 0);
    CHECK_STR(lc_str_cstr(&res.out), "hello\n");
    CHECK_STR(lc_str_cstr(&res.err), "warn\n");
    CHECK_STR(lc_str_cstr(&res.result), "42\n");
    CHECK(res.cpu_ms >= 0 && res.cpu_ms < 1000);
    CHECK(res.wall_ms >= 0 && res.wall_ms < 5000);
    CHECK(res.max_rss_kb > 0);
}

static void test_args_and_default_argv(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "args", "one two", "");
    if (run_ok(&s))
        CHECK_STR(lc_str_cstr(&res.out), "one two\n\n");
    /* Without argv the program gets just its path, so misbehave runs its
     * default mode, "ok". */
    s.argv = NULL;
    if (run_ok(&s))
        CHECK_STR(lc_str_cstr(&res.result), "42\n");
}

static void test_exit_codes(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "exit", "3", NULL);
    if (run_ok(&s)) {
        CHECK_INT(res.exited, 1);
        CHECK_INT(res.exit_code, 3);
        CHECK_INT(res.signal, 0);
    }
    spec_for(&s, argv, "segv", NULL, NULL);
    if (run_ok(&s)) {
        CHECK_INT(res.exited, 0);
        CHECK_INT(res.signal, SIGSEGV);
    }
    spec_for(&s, argv, "abort", NULL, NULL);
    if (run_ok(&s))
        CHECK_INT(res.signal, SIGABRT);
}

/* The plan's "throws in C++" fixture. */
static void test_cpp_throw(void)
{
    lc_run_spec s;

    memset(&s, 0, sizeof s);
    s.path = THROW;
    s.wall_ms = 10000;
    if (!run_ok(&s))
        return;
    CHECK_INT(res.exited, 0);
    CHECK_INT(res.signal, SIGABRT);
    /* libstdc++ and libc++ both say what was thrown. */
    CHECK(res.err.len > 0);
}

/* "spins forever": killed at the deadline, the whole group. */
static void test_deadline(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "spin", NULL, NULL);
    s.wall_ms = 300;
    if (run_ok(&s)) {
        CHECK_INT(res.timed_out, 1);
        CHECK_INT(res.signal, SIGKILL);
        CHECK(res.wall_ms >= 300 && res.wall_ms < 3000);
        CHECK(res.cpu_ms > 100);
    }
    spec_for(&s, argv, "sleep", NULL, NULL);
    s.wall_ms = 200;
    if (run_ok(&s)) {
        CHECK_INT(res.timed_out, 1);
        CHECK_INT(res.signal, SIGKILL);
        CHECK(res.cpu_ms < 100);
    }
    /* Closing every pipe doesn't end the run early: it still runs until
     * the deadline. */
    spec_for(&s, argv, "close-spin", NULL, NULL);
    s.wall_ms = 300;
    if (run_ok(&s)) {
        CHECK_INT(res.timed_out, 1);
        CHECK(res.wall_ms >= 300);
    }
}

/* RLIMIT_CPU stops a spinning program even without a deadline. */
static void test_cpu_limit(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "spin", NULL, NULL);
    s.cpu_sec = 1;
    if (run_ok(&s)) {
        CHECK_INT(res.timed_out, 0);
        CHECK(res.signal == SIGXCPU || res.signal == SIGKILL);
        CHECK(res.cpu_ms >= 900);
    }
    /* CPU time is measured for programs that finish too. */
    spec_for(&s, argv, "cpu", "300", NULL);
    if (run_ok(&s)) {
        CHECK_INT(res.exit_code, 0);
        CHECK_STR(lc_str_cstr(&res.result), "done\n");
        CHECK(res.cpu_ms >= 250);
    }
}

/* "allocates 2 GB", scaled down: what matters is that the peak is
 * measured and that the address-space guard stops it (macOS has no
 * RLIMIT_AS; the runner watches the program's memory there instead). */
static void test_memory(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "alloc", "2048", NULL);
    s.as_bytes = 192ull * LC_MIB;
    if (!run_ok(&s))
        return;
    CHECK(res.max_rss_kb > 100 * 1024);
    CHECK(res.max_rss_kb < 1024 * 1024);
#if defined(__APPLE__)
    CHECK_INT(res.mem_killed, 1);
    CHECK_INT(res.signal, SIGKILL);
#else
    CHECK_INT(res.mem_killed, 0);
    CHECK_INT(res.signal, SIGABRT); /* malloc failed */
#endif
}

/* "prints 100 MB": stopped at the cap, which is all that is kept. */
static void test_output_caps(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "flood", "100", "1");
    if (run_ok(&s)) {
        CHECK_INT(res.overflow, LC_RUN_STDOUT);
        CHECK_INT(res.out.len, LC_RUN_OUTPUT_MAX);
        CHECK_INT(res.signal, SIGKILL);
        CHECK(res.wall_ms < 5000);
    }
    spec_for(&s, argv, "flood", "100", "2");
    s.stderr_max = 1000;
    if (run_ok(&s)) {
        CHECK_INT(res.overflow, LC_RUN_STDERR);
        CHECK_INT(res.err.len, 1000);
        CHECK_INT(strlen(res.err.data), 1000);
    }
    spec_for(&s, argv, "flood", "100", "3");
    s.result_max = 3 * LC_MIB;
    if (run_ok(&s)) {
        CHECK_INT(res.overflow, LC_RUN_RESULT);
        CHECK_INT(res.result.len, 3 * LC_MIB);
    }
    /* Exactly at the cap is fine. */
    spec_for(&s, argv, "flood", "1", "1");
    if (run_ok(&s)) {
        CHECK_INT(res.overflow, 0);
        CHECK_INT(res.out.len, LC_MIB);
        CHECK_INT(res.exit_code, 0);
    }
}

/* Big input and big output at once don't deadlock. */
static void test_cat(void)
{
    lc_run_spec s;
    char *argv[6];
    size_t n = 4 * LC_MIB, i;
    char *input = malloc(n);

    CHECK(input != NULL);
    if (!input)
        return;
    for (i = 0; i < n; i++)
        input[i] = (char)('a' + i % 26);
    spec_for(&s, argv, "cat", NULL, NULL);
    s.input = input;
    s.input_len = n;
    s.stdout_max = 8 * LC_MIB;
    if (run_ok(&s)) {
        CHECK_INT(res.exit_code, 0);
        CHECK_INT(res.out.len, n);
        CHECK(res.out.len == n && memcmp(res.out.data, input, n) == 0);
    }
    /* No input: stdin is at EOF straight away. */
    s.input = NULL;
    s.input_len = 0;
    if (run_ok(&s)) {
        CHECK_INT(res.exit_code, 0);
        CHECK_INT(res.out.len, 0);
    }
    free(input);
}

/* "closes stdin early": no hang, and no SIGPIPE for us (it's ignored, as
 * front-ends must). */
static void test_stdin_unread(void)
{
    lc_run_spec s;
    char *argv[6];
    size_t n = 8 * LC_MIB;
    char *input = calloc(1, n);

    CHECK(input != NULL);
    if (!input)
        return;
    spec_for(&s, argv, "close-stdin", NULL, NULL);
    s.input = input;
    s.input_len = n;
    if (run_ok(&s)) {
        CHECK_INT(res.exit_code, 0);
        CHECK_STR(lc_str_cstr(&res.result), "closed\n");
        CHECK(res.wall_ms < 5000);
    }
    spec_for(&s, argv, "ignore-stdin", NULL, NULL);
    s.input = input;
    s.input_len = n;
    if (run_ok(&s)) {
        CHECK_INT(res.exit_code, 0);
        CHECK_STR(lc_str_cstr(&res.result), "ignored\n");
        CHECK(res.wall_ms < 5000);
    }
    free(input);
}

static long file_size(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0 ? (long)st.st_size : -1;
}

/* "forks a child": the run ends when the program does, though its child
 * still holds the pipes, and the child is killed with it. */
static void test_fork(void)
{
    const char *beat = TMP "/heartbeat";
    lc_run_spec s;
    char *argv[6];
    long a, b;

    unlink(beat);
    spec_for(&s, argv, "fork", beat, NULL);
    if (!run_ok(&s))
        return;
    CHECK_INT(res.exit_code, 0);
    CHECK_STR(lc_str_cstr(&res.result), "forked\n");
    CHECK_INT(res.timed_out, 0);
    CHECK(res.wall_ms < 3000);
    /* A killed child writes no more. */
    nap_ms(50);
    a = file_size(beat);
    nap_ms(300);
    b = file_size(beat);
    CHECK(a > 0);
    CHECK_INT(a, b);
    unlink(beat);
}

static volatile sig_atomic_t cancel_flag;

static void on_alarm(int sig)
{
    (void)sig;
    cancel_flag = 1;
}

/* The cancel flag, set from a signal handler as a CLI's SIGINT handler
 * would, stops the program promptly. */
static void test_cancel(void)
{
    struct sigaction sa, old;
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "spin", NULL, NULL);
    s.cancel = &cancel_flag;
    cancel_flag = 0;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_alarm;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, &old);
    alarm(1);
    if (run_ok(&s)) {
        CHECK_INT(res.cancelled, 1);
        CHECK_INT(res.timed_out, 0);
        CHECK_INT(res.signal, SIGKILL);
        CHECK(res.wall_ms >= 900 && res.wall_ms < 2000);
    }
    alarm(0);
    sigaction(SIGALRM, &old, NULL);

    /* Already set: the program is killed at once. */
    cancel_flag = 1;
    spec_for(&s, argv, "spin", NULL, NULL);
    s.cancel = &cancel_flag;
    if (run_ok(&s)) {
        CHECK_INT(res.cancelled, 1);
        CHECK(res.wall_ms < 1000);
    }
    cancel_flag = 0;
}

/* The child gets only fds 0-3, whatever the parent has open. */
static void test_fds(void)
{
    lc_run_spec s;
    char *argv[6];
    int fd = open("tests/fixtures/run/misbehave.c", O_RDONLY);
    /* Without FD_CLOEXEC, as a careless front-end might leave one. */
    int extra = fd >= 0 ? fcntl(fd, F_DUPFD, 10) : -1;

    CHECK(extra >= 10);
    if (fd >= 0)
        close(fd);
    spec_for(&s, argv, "fds", NULL, NULL);
    if (run_ok(&s))
        CHECK_STR(lc_str_cstr(&res.out), "0 1 2 3\n");
    s.result_fd = 0;
    if (run_ok(&s))
        CHECK_STR(lc_str_cstr(&res.out), "0 1 2\n");
    if (extra >= 0)
        close(extra);
}

/* Running doesn't leak descriptors in the parent. */
static void test_no_fd_leak(void)
{
    lc_run_spec s;
    char *argv[6];
    int before = count_open_fds(), i;

    for (i = 0; i < 20; i++) {
        spec_for(&s, argv, i % 2 ? "ok" : "segv", NULL, NULL);
        run_ok(&s);
    }
    spec_for(&s, argv, "flood", "100", "1");
    run_ok(&s);
    s.path = TMP "/missing";
    s.argv = NULL;
    CHECK_INT(lc_run(ctx, &s, &res), LC_ERR_NOTFOUND);
    CHECK_INT(count_open_fds(), before);
}

static unsigned long long expected_limit(int res_id, unsigned long long want)
{
    struct rlimit rl;

    if (getrlimit(res_id, &rl) == 0 && rl.rlim_max != RLIM_INFINITY &&
        want > rl.rlim_max)
        return rl.rlim_max;
    return want;
}

static void test_limits(void)
{
    lc_run_spec s;
    char *argv[6];
    char want[512];

    spec_for(&s, argv, "limits", NULL, NULL);
    s.cpu_sec = 5;
    s.fsize_bytes = LC_MIB;
    s.stack_bytes = 256ull * LC_MIB; /* above macOS's hard limit of 64 MiB */
    s.as_bytes = 512ull * LC_MIB;
    if (!run_ok(&s))
        return;
    snprintf(want, sizeof want, "core=0\ncpu=5\nfsize=%llu\nstack=%llu\n",
             (unsigned long long)LC_MIB,
             expected_limit(RLIMIT_STACK, 256ull * LC_MIB));
    CHECK(strncmp(lc_str_cstr(&res.out), want, strlen(want)) == 0);
    if (strncmp(lc_str_cstr(&res.out), want, strlen(want)) != 0)
        fprintf(stderr, "    got:\n%s    want:\n%s", lc_str_cstr(&res.out),
                want);
#if !defined(__APPLE__)
    CHECK(strstr(lc_str_cstr(&res.out), "as=536870912\n") != NULL);
#endif

    /* RLIMIT_FSIZE turns a big file into SIGXFSZ. */
    spec_for(&s, argv, "bigfile", TMP "/big", "2");
    s.fsize_bytes = LC_MIB;
    if (run_ok(&s))
        CHECK_INT(res.signal, SIGXFSZ);
    unlink(TMP "/big");
}

/* Ignored and blocked signals don't reach the program. */
static void test_signals_reset(void)
{
    struct sigaction ign, old_int;
    sigset_t block, old_mask;
    lc_run_spec s;
    char *argv[6];

    memset(&ign, 0, sizeof ign);
    ign.sa_handler = SIG_IGN;
    sigemptyset(&ign.sa_mask);
    sigaction(SIGINT, &ign, &old_int);
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block, &old_mask);

    spec_for(&s, argv, "signals", NULL, NULL);
    if (run_ok(&s))
        CHECK_STR(lc_str_cstr(&res.out),
                  "pipe=default int=default blocked=no\n");

    sigprocmask(SIG_SETMASK, &old_mask, NULL);
    sigaction(SIGINT, &old_int, NULL);

    spec_for(&s, argv, "pgid", NULL, NULL);
    if (run_ok(&s))
        CHECK_STR(lc_str_cstr(&res.out), "leader\n");
}

static void test_cwd(void)
{
    lc_run_spec s;
    char *argv[6];
    lc_arena a = {0};
    char *real = lc_realpath(&a, TMP);
    lc_str want = LC_STR_INIT;

    CHECK(real != NULL);
    if (!real)
        return;
    lc_str_printf(&want, "%s\n", real);
    /* The program's path is relative to our directory, not the new one. */
    spec_for(&s, argv, "cwd", NULL, NULL);
    s.cwd = TMP;
    if (run_ok(&s))
        CHECK_STR(lc_str_cstr(&res.out), lc_str_cstr(&want));
    lc_str_free(&want);
    lc_arena_free(&a);
}

static void test_start_errors(void)
{
    lc_run_spec s;
    char *argv[6];
    const char *msg;

    spec_for(&s, argv, "ok", NULL, NULL);
    s.path = TMP "/missing";
    CHECK_INT(lc_run(ctx, &s, &res), LC_ERR_NOTFOUND);
    msg = lc_last_error(ctx);
    CHECK(starts_with(msg, "cannot run 'tests/tmp/run/missing': "));

    /* Not executable. */
    s.path = "tests/fixtures/run/misbehave.c";
    CHECK_INT(lc_run(ctx, &s, &res), LC_ERR_IO);
    CHECK(starts_with(lc_last_error(ctx),
                      "cannot run 'tests/fixtures/run/misbehave.c': "));

    /* A directory that doesn't exist. */
    spec_for(&s, argv, "ok", NULL, NULL);
    s.cwd = TMP "/nowhere";
    CHECK_INT(lc_run(ctx, &s, &res), LC_ERR_NOTFOUND);
    CHECK(starts_with(lc_last_error(ctx),
                      "cannot run '" MISBEHAVE "': cannot change to "
                      "directory 'tests/tmp/run/nowhere': "));

    /* A relative path that doesn't exist, with a cwd. */
    s.path = "tests/tmp/run/missing";
    s.cwd = TMP;
    CHECK_INT(lc_run(ctx, &s, &res), LC_ERR_NOTFOUND);

    s.path = NULL;
    CHECK_INT(lc_run(ctx, &s, &res), LC_ERR_INVALID);
}

/* A result reused across runs starts clean each time. */
static void test_reuse(void)
{
    lc_run_spec s;
    char *argv[6];

    spec_for(&s, argv, "flood", "100", "1");
    run_ok(&s);
    spec_for(&s, argv, "exit", "0", NULL);
    if (run_ok(&s)) {
        CHECK_INT(res.overflow, 0);
        CHECK_INT(res.out.len, 0);
        CHECK_INT(res.signal, 0);
        CHECK_INT(res.exited, 1);
    }
}

int main(void)
{
    signal(SIGPIPE, SIG_IGN); /* as every front-end must */
    ctx = lc_open(NULL);
    lc_mkdir_p(ctx, TMP, 0777);
    lc_run_result_init(&res);

    RUN(test_ok);
    RUN(test_args_and_default_argv);
    RUN(test_exit_codes);
    RUN(test_cpp_throw);
    RUN(test_deadline);
    RUN(test_cpu_limit);
    RUN(test_memory);
    RUN(test_output_caps);
    RUN(test_cat);
    RUN(test_stdin_unread);
    RUN(test_fork);
    RUN(test_cancel);
    RUN(test_fds);
    RUN(test_no_fd_leak);
    RUN(test_limits);
    RUN(test_signals_reset);
    RUN(test_cwd);
    RUN(test_start_errors);
    RUN(test_reuse);

    lc_run_result_free(&res);
    lc_close(ctx);
    return test_report();
}
