/* nums2 holds distinct values, and nums1 a shuffled selection of them.
 * Small cases: up to 8 values, drawn from 0..n-1, from 0..15 or from
 * 0..10^4, in random, increasing or decreasing order. Full-size cases:
 * 1000 values that always include 0 and 10^4, in a shape the seed picks:
 * random; decreasing (every answer -1, the deepest stack, and nums1 is all
 * of nums2, the slowest case for a naive search); increasing; decreasing
 * with the maximum moved to the end (every other answer is that last
 * value); or a rising sawtooth, increasing values cut into blocks that
 * are each reversed, so that the answer for a value lies past the smaller
 * rest of its block, and is not the largest value to its right. */
static int up(const void *x, const void *y)
{
    int a = *(const int *)x, b = *(const int *)y;

    return (a > b) - (a < b);
}

static int down(const void *x, const void *y)
{
    return up(y, x);
}

/* Fills a[0..n) with distinct values from 0..top, n <= top + 1. */
static void distinct(int *a, int n, int top)
{
    int *pool = malloc((size_t)(top + 1) * sizeof *pool), i;

    for (i = 0; i <= top; i++)
        pool[i] = i;
    rnd_shuffle(pool, top + 1);
    for (i = 0; i < n; i++)
        a[i] = pool[i];
    free(pool);
}

void gen(int small)
{
    int n2 = small ? (int)rnd(1, 8) : 1000, n1, shape, top, i, k, end, t;
    int *nums2 = malloc((size_t)n2 * sizeof *nums2);
    int *nums1 = malloc((size_t)n2 * sizeof *nums1);

    if (small) {
        k = (int)rnd(0, 2);
        top = k == 0 ? n2 - 1 : k == 1 ? 15 : 10000;
        distinct(nums2, n2, top);
        shape = (int)rnd(0, 3);
        if (shape == 1)
            qsort(nums2, (size_t)n2, sizeof *nums2, up);
        else if (shape == 2)
            qsort(nums2, (size_t)n2, sizeof *nums2, down);
        n1 = (int)rnd(1, n2);
    } else {
        /* 998 values from 1..9999, then 0 and 10^4. */
        distinct(nums2, n2 - 2, 9998);
        for (i = 0; i < n2 - 2; i++)
            nums2[i]++;
        nums2[n2 - 2] = 0;
        nums2[n2 - 1] = 10000;
        rnd_shuffle(nums2, n2);
        shape = (int)rnd(0, 4);
        if (shape == 1 || shape == 3)
            qsort(nums2, (size_t)n2, sizeof *nums2, down);
        else if (shape == 2)
            qsort(nums2, (size_t)n2, sizeof *nums2, up);
        if (shape == 3) {
            for (i = 0; i < n2 - 1; i++)
                nums2[i] = nums2[i + 1];
            nums2[n2 - 1] = 10000;
        } else if (shape == 4) {
            qsort(nums2, (size_t)n2, sizeof *nums2, up);
            for (i = 0; i < n2; i = end) {
                end = i + (int)rnd(1, 50);
                if (end > n2)
                    end = n2;
                for (k = 0; i + k < end - 1 - k; k++) {
                    t = nums2[i + k];
                    nums2[i + k] = nums2[end - 1 - k];
                    nums2[end - 1 - k] = t;
                }
            }
        }
        n1 = shape == 1 ? n2 : (int)rnd(1, n2);
    }
    for (i = 0; i < n2; i++)
        nums1[i] = nums2[i];
    rnd_shuffle(nums1, n2);
    put_ints(nums1, n1);
    put_ints(nums2, n2);
    free(nums1);
    free(nums2);
}
