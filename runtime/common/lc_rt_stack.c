/*
 * lc_rt_stack.c: tells a stack overflow from other crashes
 * (docs/judging.md, "Verdicts"). lc compiles it with every solution, as a
 * translation unit of its own (in C++ builds as lc_rt_stack.cpp, so it is
 * written in the common subset of C11 and C++11): the POSIX headers it
 * needs then stay out of the solution's, which may define a function
 * kill or a type stack_t of its own, as on LeetCode.
 *
 * Before main(), it gives the program a handler for SIGSEGV and SIGBUS
 * that runs on a stack of its own. When the address that faulted lies at
 * the end of the main thread's stack, where a recursion too deep gets to,
 * the handler writes
 *
 *     lc-rt stack-overflow SIZE
 *
 * on fd 3, the descriptor of the result, which the harness writes only
 * once the solution has returned; SIZE is the stack's size in bytes. Then
 * the program dies of the signal, as it would have without the handler,
 * and lc reports the crash as a stack overflow.
 *
 * Built with AddressSanitizer, which handles the fault itself and reports
 * a stack overflow as one, this does nothing.
 */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#if defined(__SANITIZE_ADDRESS__)
#define LC_RT_STACK_ASAN 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define LC_RT_STACK_ASAN 1
#endif
#endif

#if !defined(LC_RT_STACK_ASAN) && (defined(__GNUC__) || defined(__clang__))

/* How far below the stack's limit a fault still counts as reaching it:
 * the frame being made when the stack ran out can be large (a local
 * array), and Linux keeps a gap of 1 MB under the stack. */
#define LC_RT_STACK_BELOW (64ull * 1024 * 1024)

static uintptr_t lc_rt_stack_top;   /* about where the stack starts */
static uintptr_t lc_rt_stack_size;  /* its limit; 0: not known */
static char lc_rt_stack_msg[64];    /* made beforehand: the handler may
                                       only make async-signal-safe calls */
static size_t lc_rt_stack_len;

static void lc_rt_stack_fault(int sig, siginfo_t *si, void *uc)
{
    uintptr_t a = (uintptr_t)si->si_addr;

    (void)uc;
    if (lc_rt_stack_size && a < lc_rt_stack_top &&
        lc_rt_stack_top - a <= lc_rt_stack_size + LC_RT_STACK_BELOW) {
        ssize_t w = write(3, lc_rt_stack_msg, lc_rt_stack_len);

        (void)w;
    }
    /* SA_RESETHAND put back the default, which this ends the program
     * with; a fault would also happen again on return. */
    raise(sig);
}

__attribute__((constructor, used)) static void lc_rt_stack_init(void)
{
    char here;
    struct rlimit rl;
    struct sigaction sa;
    stack_t ss;
    size_t size = 65536;
    int n;

    if (getrlimit(RLIMIT_STACK, &rl) != 0 || rl.rlim_cur == RLIM_INFINITY)
        return;
    if ((size_t)SIGSTKSZ > size)
        size = (size_t)SIGSTKSZ;
    memset(&ss, 0, sizeof ss);
    ss.ss_sp = malloc(size);
    ss.ss_size = size;
    if (!ss.ss_sp || sigaltstack(&ss, NULL) != 0)
        return;
    n = snprintf(lc_rt_stack_msg, sizeof lc_rt_stack_msg,
                 "lc-rt stack-overflow %llu\n",
                 (unsigned long long)rl.rlim_cur);
    if (n <= 0 || (size_t)n >= sizeof lc_rt_stack_msg)
        return;
    lc_rt_stack_len = (size_t)n;
    lc_rt_stack_top = (uintptr_t)&here;
    lc_rt_stack_size = (uintptr_t)rl.rlim_cur;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = lc_rt_stack_fault;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_RESETHAND;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
}

#else

/* ISO C wants something in every translation unit. */
typedef int lc_rt_stack_unused;

#endif
