/*
 * twosum.c: a hand-written harness and solution for the Two Sum fixture
 * pack (tests/fixtures/problems/0001-two-sum). Like the harnesses lc will
 * generate, it reads the case's inputs in the wire format on stdin and
 * writes its result on fd 3 (docs/wire.md). $LC_FIXTURE_MODE says how to
 * behave, for tests/unit/test_judge.c:
 *
 *   correct (default)  the right pair, smaller index first
 *   reversed           the right pair, larger index first
 *   wrong              always [0,0]
 *   crash-negative     crashes (SIGSEGV) when nums has a negative value
 *   spin               loops forever
 *   alloc              allocates and touches 2 GB
 *   flood              prints 100 MB
 *   silent             exits without writing a result
 *   garbage            writes a result that isn't valid wire format
 *   extra              writes a valid result followed by more text
 *   exit1              writes the right result, then exits with status 1
 *   chatty             prints on stdout and stderr, then answers right
 *   verbose            prints 100 KB on stdout and on stderr, then
 *                      answers right
 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MB (1024 * 1024)

/* The busy loop counts here, so the compiler can't remove it. */
static volatile unsigned long counter;

int main(void)
{
    const char *mode = getenv("LC_FIXTURE_MODE");
    int n, target, i, j, a = -1, b = -1, *nums;

    if (!mode)
        mode = "correct";
    if (scanf("%d", &n) != 1 || n < 0)
        return 2;
    nums = malloc((size_t)(n ? n : 1) * sizeof *nums);
    if (!nums)
        return 2;
    for (i = 0; i < n; i++)
        if (scanf("%d", &nums[i]) != 1)
            return 2;
    if (scanf("%d", &target) != 1)
        return 2;

    if (strcmp(mode, "crash-negative") == 0) {
        for (i = 0; i < n; i++) {
            if (nums[i] < 0) {
                /* Volatile store through a volatile null pointer: a real
                 * SIGSEGV that no compiler can remove (see misbehave.c). */
                volatile int *volatile p = NULL;
                *p = 1;
            }
        }
    } else if (strcmp(mode, "spin") == 0) {
        for (;;)
            counter++;
    } else if (strcmp(mode, "alloc") == 0) {
        long m, k;
        for (m = 0; m < 2048; m++) {
            volatile char *p = malloc(MB);
            if (!p)
                abort();
            for (k = 0; k < MB; k += 4096)
                p[k] = (char)(m + k);
        }
    } else if (strcmp(mode, "flood") == 0) {
        long k;
        for (k = 0; k < 100L * MB / 64; k++)
            fputs("0123456789012345678901234567890123456789012345678901234567890123", stdout);
    } else if (strcmp(mode, "silent") == 0) {
        return 0;
    } else if (strcmp(mode, "garbage") == 0) {
        dprintf(3, "2 0 x\n");
        return 0;
    } else if (strcmp(mode, "extra") == 0) {
        dprintf(3, "2 0 1 7\n");
        return 0;
    } else if (strcmp(mode, "chatty") == 0) {
        printf("n = %d\n", n);
        fprintf(stderr, "target = %d\n", target);
    } else if (strcmp(mode, "verbose") == 0) {
        long k;
        for (k = 0; k < 100 * 1024 / 16; k++) {
            printf("stdout line %3ld\n", k % 1000);
            fprintf(stderr, "stderr line %3ld\n", k % 1000);
        }
    }

    for (i = 0; i < n && a < 0; i++) {
        for (j = i + 1; j < n; j++) {
            if (nums[i] + nums[j] == target) {
                a = i;
                b = j;
                break;
            }
        }
    }
    if (strcmp(mode, "wrong") == 0)
        a = b = 0;
    if (strcmp(mode, "reversed") == 0) {
        int t = a;
        a = b;
        b = t;
    }
    fflush(stdout);
    dprintf(3, "2 %d %d\n", a, b);
    free(nums);
    return strcmp(mode, "exit1") == 0 ? 1 : 0;
}
