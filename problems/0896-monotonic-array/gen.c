/* Small cases have 1 to 8 values from -3 to 3, now and then -10^5 or 10^5,
 * so that level steps are common. Full-size ones have 10^5 values most of
 * the time, else a random number. The seed picks the shape:
 *   - random values (nearly always a climb and a drop early on);
 *   - sorted, so the answer is true and the whole array must be read;
 *   - sorted, from only a few different values: long level runs;
 *   - sorted, then the last value set below the one before it, so that
 *     the only wrong step is the last one;
 *   - sorted, then one value somewhere set below the one before it;
 *   - all equal.
 * Then, at random, every value is negated (sorted becomes the other way)
 * and the array reversed (a wrong last step becomes a wrong first step). */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

static int value(int small, int few, const int *pool)
{
    if (few)
        return pool[rnd(0, 2)];
    if (!small)
        return (int)rnd(-100000, 100000);
    if (rnd_chance(10))
        return rnd_chance(50) ? 100000 : -100000;
    return (int)rnd(-3, 3);
}

void gen(int small)
{
    int n, shape = (int)rnd(0, 5), i, p, t, lo, pool[3];
    int *nums;

    if (small)
        n = (int)rnd(1, 8);
    else if (rnd_chance(70))
        n = 100000;
    else
        n = (int)rnd(1, 100000);
    for (i = 0; i < 3; i++)
        pool[i] = value(small, 0, pool);
    nums = malloc((size_t)n * sizeof *nums);
    for (i = 0; i < n; i++)
        nums[i] = shape == 5 ? pool[0] : value(small, shape == 2, pool);
    if (shape >= 1 && shape <= 4)
        qsort(nums, (size_t)n, sizeof *nums, cmp_int);
    if (shape >= 3 && n >= 2) {
        p = shape == 3 ? n - 1 : (int)rnd(1, n - 1);
        lo = small ? nums[p - 1] - 5 : -100000;
        if (lo < -100000)
            lo = -100000;
        if (nums[p - 1] > -100000)
            nums[p] = (int)rnd(lo, nums[p - 1] - 1);
    }
    if (rnd_chance(50))
        for (i = 0; i < n; i++)
            nums[i] = -nums[i];
    if (rnd_chance(50))
        for (i = 0; i < n / 2; i++) {
            t = nums[i];
            nums[i] = nums[n - 1 - i];
            nums[n - 1 - i] = t;
        }
    put_ints(nums, n);
    free(nums);
}
