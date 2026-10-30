/*
 * misbehave.c: a program that behaves, or misbehaves, in the one way
 * argv[1] names. tests/unit/test_runner.c runs it to check how the runner
 * copes; it is not part of lc.
 *
 *   ok                 "hello" on stdout, "warn" on stderr, "42" on fd 3
 *   args A B...        prints its arguments on stdout, one per line
 *   exit N             exits with status N
 *   segv | abort       dies of SIGSEGV or SIGABRT
 *   spin               loops forever
 *   cpu MS             spins for MS ms of CPU time, then "done" on fd 3
 *   sleep              sleeps forever
 *   alloc MB           allocates and touches MB megabytes, 1 MB at a time
 *   flood MB FD        writes MB megabytes to fd FD
 *   close-stdin        closes stdin unread, then "closed" on fd 3
 *   ignore-stdin       exits without reading stdin, "ignored" on fd 3
 *   cat                copies stdin to stdout
 *   fork FILE          leaves a child running that appends to FILE every
 *                      10 ms, then "forked" on fd 3 and exits
 *   close-spin         closes fds 1, 2 and 3, then loops forever
 *   cwd                prints its working directory
 *   fds                prints the open descriptors among 0-255
 *   limits             prints its soft resource limits
 *   signals            prints the disposition of SIGPIPE and SIGINT, and
 *                      whether any signal is blocked
 *   pgid               prints "leader" if it leads its process group
 *   bigfile FILE MB    writes MB megabytes to FILE
 */
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MB (1024 * 1024)

/* Busy loops count here, so the compiler can't remove them. */
static volatile unsigned long counter;

static void nap_ms(long ms)
{
    struct timespec ts;

    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    while (nanosleep(&ts, &ts) != 0 && errno == EINTR)
        ;
}

static void spin(void)
{
    for (;;)
        counter++;
}

static int write_all(int fd, const char *p, size_t n)
{
    while (n > 0) {
        ssize_t w = write(fd, p, n);
        if (w < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        p += w;
        n -= (size_t)w;
    }
    return 0;
}

static void put_limit(const char *name, int res)
{
    struct rlimit rl;

    if (getrlimit(res, &rl) != 0)
        printf("%s=error\n", name);
    else if (rl.rlim_cur == RLIM_INFINITY)
        printf("%s=inf\n", name);
    else
        printf("%s=%llu\n", name, (unsigned long long)rl.rlim_cur);
}

static const char *disposition(int sig)
{
    struct sigaction sa;

    if (sigaction(sig, NULL, &sa) != 0)
        return "error";
    if (sa.sa_handler == SIG_DFL)
        return "default";
    if (sa.sa_handler == SIG_IGN)
        return "ignored";
    return "caught";
}

int main(int argc, char **argv)
{
    const char *mode = argc > 1 ? argv[1] : "ok";
    const char *arg = argc > 2 ? argv[2] : "0";

    if (strcmp(mode, "ok") == 0) {
        fputs("hello\n", stdout);
        fputs("warn\n", stderr);
        dprintf(3, "42\n");
        return 0;
    }
    if (strcmp(mode, "args") == 0) {
        int i;
        for (i = 2; i < argc; i++)
            printf("%s\n", argv[i]);
        return 0;
    }
    if (strcmp(mode, "exit") == 0)
        return atoi(arg);
    if (strcmp(mode, "segv") == 0) {
        /* The pointer is volatile, so the compiler can't see it is null
         * and turn the store into a trap instruction; so is the store, so
         * it can't be dropped as dead (gcc 13 drops it otherwise). */
        volatile int *volatile p = NULL;
        *p = 1;
        return 0;
    }
    if (strcmp(mode, "abort") == 0)
        abort();
    if (strcmp(mode, "spin") == 0)
        spin();
    if (strcmp(mode, "cpu") == 0) {
        clock_t until = (clock_t)(atol(arg) * (CLOCKS_PER_SEC / 1000));
        while (clock() < until)
            counter++;
        dprintf(3, "done\n");
        return 0;
    }
    if (strcmp(mode, "sleep") == 0) {
        for (;;)
            pause();
    }
    if (strcmp(mode, "alloc") == 0) {
        long i, n = atol(arg);
        for (i = 0; i < n; i++) {
            /* volatile, so the stores can't be optimized away */
            volatile char *p = malloc(MB);
            long k;
            if (!p)
                abort();
            for (k = 0; k < MB; k += 4096)
                p[k] = (char)(i + k);
        }
        return 0;
    }
    if (strcmp(mode, "flood") == 0) {
        static char buf[65536];
        long i, n = atol(arg) * (MB / (long)sizeof buf);
        int fd = argc > 3 ? atoi(argv[3]) : 1;
        memset(buf, 'x', sizeof buf);
        for (i = 0; i < n; i++)
            if (write_all(fd, buf, sizeof buf) != 0)
                return 1;
        return 0;
    }
    if (strcmp(mode, "close-stdin") == 0) {
        close(0);
        dprintf(3, "closed\n");
        return 0;
    }
    if (strcmp(mode, "ignore-stdin") == 0) {
        dprintf(3, "ignored\n");
        return 0;
    }
    if (strcmp(mode, "cat") == 0) {
        static char buf[65536];
        for (;;) {
            ssize_t n = read(0, buf, sizeof buf);
            if (n < 0 && errno == EINTR)
                continue;
            if (n <= 0)
                break;
            if (write_all(1, buf, (size_t)n) != 0)
                return 1;
        }
        return 0;
    }
    if (strcmp(mode, "fork") == 0) {
        struct stat st;
        pid_t c = fork();
        if (c < 0)
            return 1;
        if (c == 0) {
            /* Keeps stdout, stderr and fd 3 open, so the runner's pipes
             * don't close when the parent exits. */
            for (;;) {
                int fd = open(arg, O_WRONLY | O_CREAT | O_APPEND, 0666);
                if (fd >= 0) {
                    write_all(fd, "x", 1);
                    close(fd);
                }
                nap_ms(10);
            }
        }
        while (stat(arg, &st) != 0 || st.st_size == 0)
            nap_ms(5);
        dprintf(3, "forked\n");
        return 0;
    }
    if (strcmp(mode, "close-spin") == 0) {
        close(1);
        close(2);
        close(3);
        spin();
    }
    if (strcmp(mode, "cwd") == 0) {
        char buf[4096];
        if (!getcwd(buf, sizeof buf))
            return 1;
        printf("%s\n", buf);
        return 0;
    }
    if (strcmp(mode, "fds") == 0) {
        int fd, first = 1;
        for (fd = 0; fd < 256; fd++) {
            if (fcntl(fd, F_GETFD) != -1) {
                printf(first ? "%d" : " %d", fd);
                first = 0;
            }
        }
        printf("\n");
        return 0;
    }
    if (strcmp(mode, "limits") == 0) {
        put_limit("core", RLIMIT_CORE);
        put_limit("cpu", RLIMIT_CPU);
        put_limit("fsize", RLIMIT_FSIZE);
        put_limit("stack", RLIMIT_STACK);
#ifdef RLIMIT_AS
        put_limit("as", RLIMIT_AS);
#endif
        return 0;
    }
    if (strcmp(mode, "signals") == 0) {
        sigset_t set;
        int blocked = 0;
        sigprocmask(SIG_SETMASK, NULL, &set);
        blocked = sigismember(&set, SIGUSR1) || sigismember(&set, SIGINT) ||
                  sigismember(&set, SIGTERM);
        printf("pipe=%s int=%s blocked=%s\n", disposition(SIGPIPE),
               disposition(SIGINT), blocked ? "yes" : "no");
        return 0;
    }
    if (strcmp(mode, "pgid") == 0) {
        printf("%s\n", getpgrp() == getpid() ? "leader" : "member");
        return 0;
    }
    if (strcmp(mode, "bigfile") == 0) {
        static char buf[65536];
        long i, n = (argc > 3 ? atol(argv[3]) : 1) * (MB / (long)sizeof buf);
        int fd = open(arg, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd < 0)
            return 1;
        for (i = 0; i < n; i++)
            if (write_all(fd, buf, sizeof buf) != 0)
                return 1;
        close(fd);
        return 0;
    }
    fprintf(stderr, "misbehave: unknown mode '%s'\n", mode);
    return 2;
}
