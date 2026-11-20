/*
 * runner.c: runs one program with resource limits, writing its stdin and
 * capturing its stdout, stderr and the harness's result on fd 3
 * (docs/judging.md, "Running a program").
 *
 * The rules front-ends rely on (docs/PLAN.md section 4.1) shape this file:
 * no signal handlers and no alarm(), so the deadline is a poll() timeout;
 * only the child started here is waited for, by PID; and the program runs
 * in a process group of its own, so killing the group also kills anything
 * it started.
 *
 * Unlike the rest of the core, this file doesn't ask for strict POSIX:
 * wait4(), which reports a child's CPU time and peak memory, is a BSD
 * function that every target has but strict POSIX mode hides. glibc and
 * musl show it with _DEFAULT_SOURCE; macOS and the BSDs show everything
 * when no feature macro is defined.
 */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#if defined(__linux__)
#include <sys/syscall.h>
#endif
#if defined(__APPLE__)
#include <libproc.h>
#endif

#include "internal.h"

/* Longest the loop waits between looks at the cancel flag and the clock. */
#define SLICE_MS 50
/* How often to look at the program's memory where the runner watches it
 * itself (macOS). A program can touch a gigabyte in 50 ms. */
#define MEM_SLICE_MS 5
/* After killing the program, how long to wait for its pipes to close. */
#define GRACE_MS 100
/* Most bytes read or written per call. */
#define CHUNK 65536
/* Without closefrom() or close_range(), the child closes the descriptors
 * below this one by one. */
#define CLOSE_MAX 4096

/* What the child failed at, sent to the parent before it gives up. */
enum { STAGE_FD = 1, STAGE_CHDIR, STAGE_EXEC };

/* The streams the parent polls. */
enum { S_IN, S_OUT, S_ERR, S_RES, NSTREAMS };

/*
 * Everything the child needs, worked out before fork(): after it, the
 * child of a multithreaded front-end may only make async-signal-safe
 * calls, so it can't allocate or look anything up.
 */
typedef struct child_setup {
    const char *path;
    char *const *argv;
    char *const *envp;        /* the program's environment */
    const char *cwd;
    int fd[NSTREAMS];         /* the child's ends, by stream; -1 for none */
    int status_fd;            /* write end of the exec status pipe */
    int report_fd;            /* write end of the harness's report pipe
                                 (lc_run_spec.measure), or -1 */
    int nlimits;
    int limit_res[5];
    struct rlimit limit[5];
    int close_max;
} child_setup;

/*
 * Signals a front-end may ignore. An ignored signal stays ignored across
 * exec, which would change how the program behaves (a solution that writes
 * to a closed pipe should die of SIGPIPE), so the child resets them.
 * Caught signals are reset by exec itself.
 */
static const int reset_signals[] = {
    SIGPIPE, SIGINT, SIGQUIT, SIGHUP, SIGTERM, SIGCHLD, SIGALRM,
    SIGUSR1, SIGUSR2, SIGTSTP, SIGTTIN, SIGTTOU, SIGXCPU, SIGXFSZ
};

/* POSIX leaves declaring it to the program. */
extern char **environ;

/*
 * The time zone every program runs in: UTC, as on LeetCode's machines,
 * whatever lc's own. A solution's localtime() and mktime() then give the
 * same answers everywhere; under a zone with daylight saving time, a day
 * can be 23 or 25 hours long. "UTC0" is POSIX's form, which needs no time
 * zone files.
 */
#define RUN_TZ "TZ=UTC0"

/*
 * Whether harnesses measure the solution themselves (lc_run_spec.measure).
 * Linux counts, in a program's peak memory, the pages it inherited from
 * lc through fork(); macOS and the BSDs don't need it.
 */
#if defined(__linux__)
#define MEASURE 1
#else
#define MEASURE 0
#endif

/* Asks a harness to report on fd 4 (lc_run_spec.measure). */
#define RUN_MEASURE "LC_RT_MEASURE=4"

/* `env` (lc's environment when NULL) with TZ set to RUN_TZ, and with
 * RUN_MEASURE when `measure` (and only then), in `a`. */
static char **run_env(lc_arena *a, char *const *env, int measure)
{
    size_t n = 0, i, k = 0;
    char **out;

    if (!env)
        env = environ;
    while (env[n])
        n++;
    out = lc_arena_array(a, n + 3, sizeof *out);
    if (!out)
        return NULL;
    for (i = 0; i < n; i++)
        if (strncmp(env[i], "TZ=", 3) != 0 &&
            strncmp(env[i], "LC_RT_MEASURE=", 14) != 0)
            out[k++] = env[i];
    out[k++] = RUN_TZ;
    if (measure)
        out[k++] = RUN_MEASURE;
    out[k] = NULL;
    return out;
}

/* ------------------------------------------------------------------ */
/* In the child                                                        */
/* ------------------------------------------------------------------ */

/* Tells the parent what failed, with errno, and exits. */
static void child_fail(int fd, int stage)
{
    int msg[2];
    ssize_t n;

    msg[0] = stage;
    msg[1] = errno;
    do
        n = write(fd, msg, sizeof msg);
    while (n < 0 && errno == EINTR);
    _exit(127);
}

/* Closes every descriptor from `low` up. */
static void close_from(int low, int max)
{
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || \
    defined(__DragonFly__)
    (void)max;
    closefrom(low);
#else
    int fd;

#if defined(__linux__) && defined(SYS_close_range)
    /* Linux 5.9 and later; older kernels say ENOSYS. */
    if (syscall(SYS_close_range, (unsigned)low, ~0u, 0u) == 0)
        return;
#endif
    for (fd = low; fd < max; fd++)
        close(fd);
#endif
}

/*
 * Sets up the child's process and execs the program. Only
 * async-signal-safe calls from here on (setrlimit isn't on POSIX's list,
 * but it is a plain system call everywhere).
 */
static void run_child(const child_setup *cs)
{
    struct sigaction sa;
    sigset_t none;
    int fd[NSTREAMS], st, rep, i;

    /* A group of its own, which the parent can kill as a whole. */
    setpgid(0, 0);

    memset(&sa, 0, sizeof sa);
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    for (i = 0; i < (int)(sizeof reset_signals / sizeof reset_signals[0]); i++)
        sigaction(reset_signals[i], &sa, NULL);
    sigemptyset(&none);
    sigprocmask(SIG_SETMASK, &none, NULL);

    /*
     * The pipes go to fds 0-3, the harness's report pipe (if any) to 4
     * and the status pipe to 5. First move every one of them above 5, so
     * that no dup2() below can overwrite one that is still needed (they
     * can be low when the front-end runs with fd 0, 1 or 2 closed).
     */
    st = cs->status_fd;
    if (st <= 5) {
        st = fcntl(st, F_DUPFD, 6);
        if (st < 0)
            _exit(127);
    }
    for (i = 0; i <= NSTREAMS; i++) {
        int *f = i < NSTREAMS ? &fd[i] : &rep;

        *f = i < NSTREAMS ? cs->fd[i] : cs->report_fd;
        if (*f >= 0 && *f <= 5) {
            *f = fcntl(*f, F_DUPFD, 6);
            if (*f < 0)
                child_fail(st, STAGE_FD);
        }
    }
    for (i = 0; i < NSTREAMS; i++) {
        if (fd[i] < 0)
            close(i);
        else if (dup2(fd[i], i) < 0)
            child_fail(st, STAGE_FD);
    }
    if (rep < 0)
        close(4);
    else if (dup2(rep, 4) < 0)
        child_fail(st, STAGE_FD);
    if (dup2(st, 5) < 0)
        child_fail(st, STAGE_FD);
    st = 5;
    /* Closed by a successful exec, which is how the parent learns of it. */
    fcntl(st, F_SETFD, FD_CLOEXEC);
    /* Whatever else is open: the originals above, and anything the
     * front-end left without FD_CLOEXEC. */
    close_from(6, cs->close_max);

    for (i = 0; i < cs->nlimits; i++)
        setrlimit(cs->limit_res[i], &cs->limit[i]);

    if (cs->cwd && chdir(cs->cwd) != 0)
        child_fail(st, STAGE_CHDIR);
    execve(cs->path, cs->argv, cs->envp);
    child_fail(st, STAGE_EXEC);
}

/* ------------------------------------------------------------------ */
/* In the parent                                                       */
/* ------------------------------------------------------------------ */

static long long now_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static void close_fd(int *fd)
{
    if (*fd >= 0) {
        close(*fd);
        *fd = -1;
    }
}

/*
 * A pipe with FD_CLOEXEC on both ends. Not pipe2(O_CLOEXEC), which macOS
 * lacks; a fork on another thread between the two calls can leak an end
 * into that thread's child, but lc_run's own children close every stray
 * descriptor (close_from), and the poll loop stops waiting for a pipe some
 * other process holds once the program has exited.
 */
static int cloexec_pipe(int p[2])
{
    if (pipe(p) != 0)
        return -1;
    fcntl(p[0], F_SETFD, FD_CLOEXEC);
    fcntl(p[1], F_SETFD, FD_CLOEXEC);
    return 0;
}

static void set_nonblock(int fd)
{
    int fl = fcntl(fd, F_GETFL);

    if (fl >= 0)
        fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

/* Adds a limit for the child, lowered to the current hard limit: raising
 * a hard limit needs privileges, and the attempt would fail. */
static void add_limit(child_setup *cs, int res, unsigned long long soft,
                      unsigned long long hard)
{
    struct rlimit cur;
    rlim_t s = (rlim_t)soft, h = (rlim_t)hard;

    if (soft == 0 || hard < soft)
        return;
    if (getrlimit(res, &cur) == 0 && cur.rlim_max != RLIM_INFINITY) {
        if (s > cur.rlim_max)
            s = cur.rlim_max;
        if (h > cur.rlim_max)
            h = cur.rlim_max;
    }
    cs->limit_res[cs->nlimits] = res;
    cs->limit[cs->nlimits].rlim_cur = s;
    cs->limit[cs->nlimits].rlim_max = h;
    cs->nlimits++;
}

/* RLIMIT_CORE is 0 whatever the spec says, so set it apart. */
static void add_core_limit(child_setup *cs)
{
    cs->limit_res[cs->nlimits] = RLIMIT_CORE;
    cs->limit[cs->nlimits].rlim_cur = 0;
    cs->limit[cs->nlimits].rlim_max = 0;
    cs->nlimits++;
}

/*
 * 1 once the child has finished, without reaping it: as long as it is a
 * zombie its PID, and so its process group's ID, can't be reused, which
 * makes killing the group afterwards safe. Also 1 when the child is gone
 * altogether (ECHILD: SIGCHLD is ignored), which wait4 then reports.
 */
static int child_exited(pid_t pid)
{
    siginfo_t si;

    memset(&si, 0, sizeof si);
    if (waitid(P_PID, (id_t)pid, &si, WEXITED | WNOHANG | WNOWAIT) != 0)
        return errno == ECHILD;
    return si.si_pid == pid;
}

/* macOS ignores RLIMIT_AS, so the runner watches the program's memory
 * itself there. */
#if defined(__APPLE__)
#define WATCH_MEMORY 1
#else
#define WATCH_MEMORY 0
#endif

/* 1 if the program uses more than `limit` bytes (only where WATCH_MEMORY). */
static int over_memory(pid_t pid, unsigned long long limit)
{
#if defined(__APPLE__)
    struct rusage_info_v2 ri;

    if (limit == 0 ||
        proc_pid_rusage(pid, RUSAGE_INFO_V2, (rusage_info_t *)&ri) != 0)
        return 0;
    return ri.ri_resident_size > limit || ri.ri_phys_footprint > limit;
#else
    (void)pid;
    (void)limit;
    return 0;
#endif
}

static long tv_ms(const struct timeval *tv)
{
    return (long)tv->tv_sec * 1000 + (long)(tv->tv_usec / 1000);
}

void lc_run_result_init(lc_run_result *r)
{
    memset(r, 0, sizeof *r);
    lc_str_init(&r->out);
    lc_str_init(&r->err);
    lc_str_init(&r->result);
}

void lc_run_result_free(lc_run_result *r)
{
    lc_str_free(&r->out);
    lc_str_free(&r->err);
    lc_str_free(&r->result);
    lc_run_result_init(r);
}

/* Clears the result for a new run, keeping the buffers' memory. */
static void reset_result(lc_run_result *r)
{
    lc_str out = r->out, err = r->err, res = r->result;

    memset(r, 0, sizeof *r);
    r->out = out;
    r->err = err;
    r->result = res;
    lc_str_clear(&r->out);
    lc_str_clear(&r->err);
    lc_str_clear(&r->result);
}

/* State of the poll loop. */
typedef struct run_loop {
    pid_t pid;
    int fd[NSTREAMS];         /* the parent's ends; -1 once closed */
    lc_str *buf[NSTREAMS];
    size_t cap[NSTREAMS];
    const char *input;
    size_t input_len;
    size_t written;
    int killed;
    long long grace_end;
    int err;                  /* errno when poll() failed */
    int report;               /* read end of the report pipe, or -1 */
    char rbuf[192];           /* what the harness reported so far */
    size_t rlen;
    int report_eof;           /* the harness closed the report pipe */
    int killed_solution;      /* kill_group killed only the solution's
                                 process, whose harness then reports */
} run_loop;

/* Reads what the harness has reported so far (lc_run_spec.measure). */
static void read_report(run_loop *lp)
{
    while (lp->report >= 0 && lp->rlen < sizeof lp->rbuf - 1) {
        ssize_t n = read(lp->report, lp->rbuf + lp->rlen,
                         sizeof lp->rbuf - 1 - lp->rlen);

        if (n > 0) {
            lp->rlen += (size_t)n;
        } else if (n < 0 && errno == EINTR) {
            continue;
        } else {
            if (n == 0)
                lp->report_eof = 1;
            break;
        }
    }
    lp->rbuf[lp->rlen] = '\0';
}

/* Longest lc waits for a harness's report after killing the solution's
 * process: it comes as soon as the harness is scheduled. */
#define REPORT_WAIT_MS 2000

/* Waits until the harness has written its report and exited (it closes
 * the pipe), or REPORT_WAIT_MS. */
static void await_report(run_loop *lp)
{
    long long end = now_ms() + REPORT_WAIT_MS;

    for (;;) {
        struct pollfd pfd;
        long long left = end - now_ms();

        read_report(lp);
        if (lp->report_eof || lp->rlen >= sizeof lp->rbuf - 1 || left <= 0)
            return;
        pfd.fd = lp->report;
        pfd.events = POLLIN;
        pfd.revents = 0;
        if (poll(&pfd, 1, (int)left) < 0 && errno != EINTR)
            return;
    }
}

/*
 * The numbers of a report line "<tag> N N ...", which must be followed
 * by exactly `n` numbers and a newline. Returns 1 when the line is there
 * and well formed.
 */
static int report_line(const char *buf, const char *tag, long long *v,
                       int n)
{
    size_t len = strlen(tag);
    const char *p = buf;
    int k;

    /* The line that starts with the tag and a space. */
    while (strncmp(p, tag, len) != 0 || p[len] != ' ') {
        p = strchr(p, '\n');
        if (!p)
            return 0;
        p++;
    }
    p += len;
    for (k = 0; k < n; k++) {
        long long x = 0;
        const char *s;

        if (*p++ != ' ')
            return 0;
        s = p;
        while (*p >= '0' && *p <= '9' && x < 1000000000000LL)
            x = x * 10 + (*p++ - '0');
        if (p == s)
            return 0;
        v[k] = x;
    }
    return *p == '\n';
}

/* Kills the program and everything in its group, once. */
static void kill_group(run_loop *lp, long long now)
{
    long long pid;

    if (lp->killed)
        return;
    lp->killed = 1;
    lp->grace_end = now + GRACE_MS;
    /*
     * A harness that measures the solution (lc_run_spec.measure) said
     * which process runs it. Kill that one first: the harness then
     * reports its CPU time and memory, and exits. The rest of the group
     * goes when the run is over.
     */
    read_report(lp);
    if (report_line(lp->rbuf, "lc-rt-pid", &pid, 1) && pid > 1 &&
        pid != (long long)lp->pid) {
        kill((pid_t)pid, SIGKILL);
        lp->killed_solution = 1;
        return;
    }
    kill(-lp->pid, SIGKILL);
}

/* Writes more of the input. A program that stops reading (EPIPE) just
 * doesn't get the rest. */
static void write_input(run_loop *lp)
{
    size_t n = lp->input_len - lp->written;
    ssize_t w;

    if (n > CHUNK)
        n = CHUNK;
    w = write(lp->fd[S_IN], lp->input + lp->written, n);
    if (w > 0) {
        lp->written += (size_t)w;
        if (lp->written == lp->input_len)
            close_fd(&lp->fd[S_IN]);
    } else if (w < 0 && errno != EINTR && errno != EAGAIN &&
               errno != EWOULDBLOCK) {
        close_fd(&lp->fd[S_IN]);
    }
}

/*
 * Reads what a stream has. Up to the stream's cap it goes into the buffer;
 * one byte past it marks an overflow, which kills the program, and later
 * bytes are thrown away. Returns LC_ERR_NOMEM or LC_OK.
 */
static int read_output(run_loop *lp, int s, lc_run_result *r, long long now)
{
    lc_str *b = lp->buf[s];
    unsigned bit = 1u << (s - 1);
    ssize_t n;

    if (!(r->overflow & bit)) {
        size_t want = lp->cap[s] - b->len + 1;

        if (want > CHUNK)
            want = CHUNK;
        if (lc_str_reserve(b, want) != LC_OK)
            return LC_ERR_NOMEM;
        n = read(lp->fd[s], b->data + b->len, want);
        if (n > 0) {
            b->len += (size_t)n;
            if (b->len > lp->cap[s]) {
                b->len = lp->cap[s];
                r->overflow |= bit;
                kill_group(lp, now);
            }
            b->data[b->len] = '\0';
        }
    } else {
        char sink[4096];
        n = read(lp->fd[s], sink, sizeof sink);
    }
    if (n == 0 || (n < 0 && errno != EINTR && errno != EAGAIN &&
                   errno != EWOULDBLOCK))
        close_fd(&lp->fd[s]);
    return LC_OK;
}

/* Gives up on a run after a failure in the parent: kills and reaps the
 * child, closes everything. */
static void abandon(run_loop *lp)
{
    int i;

    for (i = 0; i < NSTREAMS; i++)
        close_fd(&lp->fd[i]);
    close_fd(&lp->report);
    kill(-lp->pid, SIGKILL);
    while (waitpid(lp->pid, NULL, 0) < 0 && errno == EINTR)
        ;
}

/*
 * Puts the report of a harness that measured the solution itself (see
 * lc_run_spec.measure) in *r: how the solution ended, and the CPU time
 * and peak memory of its own process, which the counts of the harness's
 * process don't separate from what it inherited from lc. A missing or
 * malformed report (the harness was killed before it wrote one) leaves
 * *r as wait4() filled it.
 */
static void apply_report(run_loop *lp, lc_run_result *r)
{
    long long v[4];

    read_report(lp);
    if (!report_line(lp->rbuf, "lc-rt", v, 4))
        return;
    r->exited = 0;
    r->exit_code = 0;
    r->signal = 0;
    if (WIFEXITED((int)v[0])) {
        r->exited = 1;
        r->exit_code = WEXITSTATUS((int)v[0]);
    } else if (WIFSIGNALED((int)v[0])) {
        r->signal = WTERMSIG((int)v[0]);
    }
    r->max_rss_kb = (long)v[1];
    r->cpu_ms = (long)((v[2] + v[3]) / 1000);
}

/*
 * Feeds the input and drains the output until the program has finished
 * and closed its pipes, or until it is killed (deadline, cancel, an
 * overflowing stream, too much memory) and the pipes have closed or the
 * grace period is over.
 */
static int poll_loop(run_loop *lp, const lc_run_spec *spec, lc_run_result *r,
                     long long start)
{
    long long deadline = spec->wall_ms > 0 ? start + spec->wall_ms : 0;
    int nap = 1;

    for (;;) {
        struct pollfd pfd[NSTREAMS];
        int map[NSTREAMS], n = 0, i, rc, timeout;
        long long now = now_ms();

        for (i = 0; i < NSTREAMS; i++) {
            if (lp->fd[i] < 0)
                continue;
            pfd[n].fd = lp->fd[i];
            pfd[n].events = i == S_IN ? POLLOUT : POLLIN;
            pfd[n].revents = 0;
            map[n++] = i;
        }
        if (!lp->killed) {
            if (spec->cancel && *spec->cancel) {
                r->cancelled = 1;
                kill_group(lp, now);
            } else if (deadline && now >= deadline) {
                r->timed_out = 1;
                kill_group(lp, now);
            } else if (over_memory(lp->pid, spec->as_bytes)) {
                r->mem_killed = 1;
                kill_group(lp, now);
            }
        }
        if (lp->killed) {
            if (n == 0 || now >= lp->grace_end)
                return LC_OK;
            timeout = (int)(lp->grace_end - now);
        } else if (n == 0) {
            /* Every pipe is closed; the program may still be running. Its
             * descriptors close a moment before it becomes a zombie, so
             * look again soon, then less often. */
            if (child_exited(lp->pid))
                return LC_OK;
            timeout = nap;
            if (nap < SLICE_MS)
                nap *= 2;
        } else {
            timeout = SLICE_MS;
        }
        if (timeout > SLICE_MS)
            timeout = SLICE_MS;
        if (WATCH_MEMORY && spec->as_bytes && !lp->killed &&
            timeout > MEM_SLICE_MS)
            timeout = MEM_SLICE_MS;
        if (deadline && !lp->killed && deadline - now < timeout)
            timeout = (int)(deadline - now);

        rc = poll(pfd, (nfds_t)n, timeout);
        if (rc < 0) {
            if (errno == EINTR)
                continue;
            lp->err = errno;
            return LC_ERR_IO;
        }
        if (rc == 0) {
            /*
             * Quiet for a while with pipes still open: if the program has
             * exited, what holds them is something it started, or a
             * process that inherited a pipe by accident. Stop waiting for
             * them.
             */
            if (n && !lp->killed && child_exited(lp->pid))
                kill_group(lp, now_ms());
            continue;
        }
        now = now_ms();
        for (i = 0; i < n; i++) {
            if (!pfd[i].revents)
                continue;
            if (map[i] == S_IN)
                write_input(lp);
            else if (read_output(lp, map[i], r, now) != LC_OK)
                return LC_ERR_NOMEM;
        }
    }
}

/* Reports the child's setup failure, after reaping it. */
static int start_failed(lc_ctx *ctx, const lc_run_spec *spec,
                        const int msg[2])
{
    int e = msg[1];
    int status = e == ENOENT || e == ENOTDIR ? LC_ERR_NOTFOUND : LC_ERR_IO;

    switch (msg[0]) {
    case STAGE_CHDIR:
        return lc_set_errno(ctx, status, e, "cannot run '%s': cannot change "
                            "to directory '%s'", spec->path, spec->cwd);
    case STAGE_EXEC:
        return lc_set_errno(ctx, status, e, "cannot run '%s'", spec->path);
    default:
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot run '%s': cannot set "
                            "up its file descriptors", spec->path);
    }
}

int lc_run(lc_ctx *ctx, const lc_run_spec *spec, lc_run_result *r)
{
    int p[NSTREAMS][2], stp[2] = {-1, -1}, rp[2] = {-1, -1}, i, rc, status;
    int msg[2];
    char *argv0[2];
    child_setup cs;
    run_loop lp;
    lc_arena tmp = {0};
    struct rlimit nofile;
    struct rusage ru;
    long long start;
    size_t got;
    pid_t w;

    if (!spec || !spec->path || !r || (spec->input_len && !spec->input))
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_run: bad arguments");
    reset_result(r);
    for (i = 0; i < NSTREAMS; i++)
        p[i][0] = p[i][1] = -1;

    memset(&cs, 0, sizeof cs);
    cs.path = spec->path;
    /* The child changes directory before exec, so a relative path must be
     * resolved here. */
    if (spec->cwd && spec->path[0] != '/') {
        cs.path = lc_realpath(&tmp, spec->path);
        if (!cs.path) {
            int e = errno;
            lc_arena_free(&tmp);
            if (e == ENOMEM)
                return lc_oom(ctx);
            return lc_set_errno(ctx, e == ENOENT || e == ENOTDIR
                                         ? LC_ERR_NOTFOUND : LC_ERR_IO,
                                e, "cannot run '%s'", spec->path);
        }
    }
    argv0[0] = (char *)spec->path;
    argv0[1] = NULL;
    cs.argv = spec->argv ? spec->argv : argv0;
    cs.envp = run_env(&tmp, spec->envp, MEASURE && spec->measure);
    cs.cwd = spec->cwd;
    if (!cs.envp) {
        lc_arena_free(&tmp);
        return lc_oom(ctx);
    }

    add_core_limit(&cs);
    if (spec->cpu_sec > 0)
        add_limit(&cs, RLIMIT_CPU, (unsigned long long)spec->cpu_sec,
                  (unsigned long long)spec->cpu_sec + 1);
#if !defined(__APPLE__)
    add_limit(&cs, RLIMIT_AS, spec->as_bytes, spec->as_bytes);
#endif
    add_limit(&cs, RLIMIT_STACK, spec->stack_bytes, spec->stack_bytes);
    add_limit(&cs, RLIMIT_FSIZE, spec->fsize_bytes, spec->fsize_bytes);
    cs.close_max = CLOSE_MAX;
    if (getrlimit(RLIMIT_NOFILE, &nofile) == 0 &&
        nofile.rlim_cur != RLIM_INFINITY && nofile.rlim_cur < CLOSE_MAX)
        cs.close_max = (int)nofile.rlim_cur;

    for (i = 0; i < NSTREAMS; i++) {
        if (i == S_RES && !spec->result_fd)
            continue;
        if (cloexec_pipe(p[i]) != 0)
            goto pipe_failed;
    }
    if (cloexec_pipe(stp) != 0)
        goto pipe_failed;
    if (MEASURE && spec->measure && cloexec_pipe(rp) != 0)
        goto pipe_failed;
    cs.fd[S_IN] = p[S_IN][0];
    for (i = S_OUT; i < NSTREAMS; i++)
        cs.fd[i] = p[i][1];
    cs.status_fd = stp[1];
    cs.report_fd = rp[1];

    w = fork();
    if (w < 0) {
        int e = errno;
        for (i = 0; i < NSTREAMS; i++) {
            close_fd(&p[i][0]);
            close_fd(&p[i][1]);
        }
        close_fd(&stp[0]);
        close_fd(&stp[1]);
        close_fd(&rp[0]);
        close_fd(&rp[1]);
        lc_arena_free(&tmp);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot run '%s': fork",
                            spec->path);
    }
    if (w == 0)
        run_child(&cs);

    /* Also here, so the group exists whichever process gets there first. */
    setpgid(w, w);
    lc_arena_free(&tmp);
    memset(&lp, 0, sizeof lp);
    lp.pid = w;
    lp.report = rp[0];
    close_fd(&rp[1]);
    if (lp.report >= 0)
        set_nonblock(lp.report);
    lp.fd[S_IN] = p[S_IN][1];
    for (i = S_OUT; i < NSTREAMS; i++)
        lp.fd[i] = p[i][0];
    close_fd(&p[S_IN][0]);
    for (i = S_OUT; i < NSTREAMS; i++)
        close_fd(&p[i][1]);
    close_fd(&stp[1]);

    /* EOF means the exec worked (it closed the status pipe); a message
     * means the child failed and is exiting. */
    got = 0;
    for (;;) {
        ssize_t n = read(stp[0], (char *)msg + got, sizeof msg - got);
        if (n > 0) {
            got += (size_t)n;
            if (got == sizeof msg)
                break;
        } else if (n == 0 || errno != EINTR) {
            break;
        }
    }
    close_fd(&stp[0]);
    if (got) {
        if (got < sizeof msg) {
            msg[0] = STAGE_FD;
            msg[1] = EIO;
        }
        abandon(&lp);
        return start_failed(ctx, spec, msg);
    }

    start = now_ms();
    lp.input = spec->input;
    lp.input_len = spec->input_len;
    lp.buf[S_OUT] = &r->out;
    lp.buf[S_ERR] = &r->err;
    lp.buf[S_RES] = &r->result;
    lp.cap[S_OUT] = spec->stdout_max ? spec->stdout_max : LC_RUN_OUTPUT_MAX;
    lp.cap[S_ERR] = spec->stderr_max ? spec->stderr_max : LC_RUN_OUTPUT_MAX;
    lp.cap[S_RES] = spec->result_max ? spec->result_max : LC_RUN_OUTPUT_MAX;
    for (i = 0; i < NSTREAMS; i++)
        if (lp.fd[i] >= 0)
            set_nonblock(lp.fd[i]);
    if (lp.input_len == 0)
        close_fd(&lp.fd[S_IN]);

    rc = poll_loop(&lp, spec, r, start);
    if (rc != LC_OK) {
        abandon(&lp);
        if (rc == LC_ERR_NOMEM)
            return lc_oom(ctx);
        return lc_set_errno(ctx, LC_ERR_IO, lp.err, "cannot run '%s': poll",
                            spec->path);
    }
    r->wall_ms = (long)(now_ms() - start);
    for (i = 0; i < NSTREAMS; i++)
        close_fd(&lp.fd[i]);
    /* The rest of the group goes next: first the harness's report on the
     * solution that was killed. */
    if (lp.killed_solution)
        await_report(&lp);
    /* Anything still left in the group: the program itself if it was
     * killed, or what it started and left running. Safe while the program
     * is unreaped (see child_exited). */
    kill(-lp.pid, SIGKILL);

    memset(&ru, 0, sizeof ru);
    do
        w = wait4(lp.pid, &status, 0, &ru);
    while (w < 0 && errno == EINTR);
    if (w < 0) {
        int e = errno;
        if (e == ECHILD)
            return lc_set_error(ctx, LC_ERR_IO, "cannot run '%s': its exit "
                                "status was lost (is SIGCHLD ignored?)",
                                spec->path);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot run '%s': wait",
                            spec->path);
    }
    if (WIFEXITED(status)) {
        r->exited = 1;
        r->exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        r->signal = WTERMSIG(status);
    }
    r->cpu_ms = tv_ms(&ru.ru_utime) + tv_ms(&ru.ru_stime);
#if defined(__APPLE__)
    r->max_rss_kb = (long)(ru.ru_maxrss / 1024); /* bytes on macOS */
#else
    r->max_rss_kb = (long)ru.ru_maxrss;          /* KiB elsewhere */
#endif
    if (lp.report >= 0) {
        apply_report(&lp, r);
        close_fd(&lp.report);
    }
    return LC_OK;

pipe_failed:
    {
        int e = errno;
        for (i = 0; i < NSTREAMS; i++) {
            close_fd(&p[i][0]);
            close_fd(&p[i][1]);
        }
        close_fd(&stp[0]);
        close_fd(&stp[1]);
        close_fd(&rp[0]);
        close_fd(&rp[1]);
        lc_arena_free(&tmp);
        return lc_set_errno(ctx, LC_ERR_IO, e, "cannot run '%s': cannot "
                            "create a pipe", spec->path);
    }
}
