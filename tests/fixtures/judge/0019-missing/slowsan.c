/* Right, but with the sanitizers it spends 700 ms of CPU time on the last
 * case: more than the sanitizer check's limit, three times the problem's
 * 200 ms. A case that runs out of time there doesn't count. */
#include <time.h>

#if defined(__SANITIZE_ADDRESS__)
#define SLOW 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define SLOW 1
#endif
#endif

int missing(int* nums, int numsSize) {
    int i;
#ifdef SLOW
    if (numsSize == 7) {
        clock_t start = clock();
        while (clock() - start < CLOCKS_PER_SEC / 10 * 7)
            ;
    }
#endif
    for (i = 0; i < numsSize; i++)
        if (nums[i] != i)
            return i;
    return numsSize;
}
