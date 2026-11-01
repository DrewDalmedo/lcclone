/* An even number of values, in random order, sorted up or sorted down.
 * Small cases have 2 to 8 values of at most 3, 10 or 100, so that equal
 * values (within a round and across rounds) come up often. Full-size ones
 * have 100 values; the seed picks values from all of 1..100, from a few
 * values only, or a single value. */
static int cmp_up(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    int nums[100];
    int n = small ? 2 * (int)rnd(1, 4) : 100;
    int lo = 1, hi, order, t, i;

    t = (int)rnd(0, 9);
    if (small)
        hi = t < 4 ? 3 : t < 7 ? 10 : 100;
    else if (t < 7) {
        hi = 100;
    } else if (t < 9) {
        lo = (int)rnd(1, 96);
        hi = lo + 4;
    } else {
        lo = hi = (int)rnd(1, 100);
    }
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    order = (int)rnd(0, 3);
    if (order >= 2) {
        qsort(nums, (size_t)n, sizeof *nums, cmp_up);
        if (order == 3)
            for (i = 0; i < n / 2; i++) {
                t = nums[i];
                nums[i] = nums[n - 1 - i];
                nums[n - 1 - i] = t;
            }
    }
    put_ints(nums, n);
}
