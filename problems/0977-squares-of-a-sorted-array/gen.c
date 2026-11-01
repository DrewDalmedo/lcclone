/* A sorted array of random values. The seed picks the signs: all
 * negative (the squares come out reversed), all non-negative, or mixed.
 * Small cases use tiny values, so that repeats, zeros and pairs like -3
 * and 3 come up often; now and then they use the full range instead. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000, i;
    int top = small && !rnd_chance(15) ? 6 : 10000;
    int shape = (int)rnd(0, 3);
    int lo = shape == 1 ? 0 : -top, hi = shape == 0 ? -1 : top;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    qsort(nums, (size_t)n, sizeof *nums, cmp_int);
    put_ints(nums, n);
    free(nums);
}
