/* Small cases have 4 to 8 values from a narrow range the seed picks (1..3,
 * 1..10, 9995..10000 or 1..10000), so that a repeated largest or smallest
 * value, and all values equal, come up often. Full-size ones have 10^4
 * values, the seed picking the shape: random from 1..10^4; sorted up;
 * sorted down; random with 1 and 10^4 twice each at random places; or one
 * value everywhere except a single smallest value first and a single
 * largest value last. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    static const int lows[] = {1, 1, 9995, 1}, highs[] = {3, 10, 10000, 10000};
    int n = small ? (int)rnd(4, 8) : 10000;
    int lo = 1, hi = 10000, shape = 0, r, i, k, t, mid;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        r = (int)rnd(0, 3);
        lo = lows[r];
        hi = highs[r];
    } else {
        shape = (int)rnd(0, 4);
    }
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    if (shape == 1 || shape == 2) {
        qsort(nums, (size_t)n, sizeof *nums, cmp_int);
        if (shape == 2)
            for (i = 0, k = n - 1; i < k; i++, k--) {
                t = nums[i];
                nums[i] = nums[k];
                nums[k] = t;
            }
    } else if (shape == 3) {
        nums[0] = nums[1] = 1;
        nums[2] = nums[3] = 10000;
        rnd_shuffle(nums, n);
    } else if (shape == 4) {
        mid = (int)rnd(2, 9999);
        for (i = 0; i < n; i++)
            nums[i] = mid;
        nums[0] = (int)rnd(1, mid - 1);
        nums[n - 1] = (int)rnd(mid + 1, 10000);
    }
    put_ints(nums, n);
    free(nums);
}
