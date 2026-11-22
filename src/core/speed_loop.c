/*
 * speed_loop.c: the fixed loop lc times to learn how fast this machine is
 * (speed.c; docs/judging.md, "Slower machines").
 *
 * Its time is compared with a constant, so it must not depend on how lc
 * was built. The Makefile builds this one file with its own flags, without
 * CFLAGS, so -O0 or the sanitizers in a test build don't reach it. The
 * loop leaves a compiler little to choose: a load that depends on the last
 * one, which can't be vectorized, and a branch it can't turn into a
 * conditional move. Apple clang and gcc 16 at -O2 are within 12% on an M4.
 *
 * Any change here changes its time: change SPEED_VERSION and REFERENCE_US
 * in speed.c with it.
 */

#include <stdint.h>

uint64_t lc_speed_loop(const uint32_t *table, uint32_t mask, uint64_t x,
                       unsigned long n);

#if defined(__GNUC__) || defined(__clang__)
#define KEEP_BRANCH __asm__("")
#else
#define KEEP_BRANCH ((void)0)
#endif

uint64_t lc_speed_loop(const uint32_t *table, uint32_t mask, uint64_t x,
                       unsigned long n)
{
    uint64_t acc = 0;
    uint32_t i = 0;
    unsigned long k;

    for (k = 0; k < n; k++) {
        i = table[i];
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        /* A random bit: the branch predictor can't learn it. */
        if (x & 64) {
            KEEP_BRANCH;
            acc += (x >> 3) + i;
        } else {
            KEEP_BRANCH;
            acc = (acc ^ x) * 0x9E3779B97F4A7C15ull;
        }
        acc += table[(uint32_t)acc & mask];
    }
    return acc ^ i;
}
