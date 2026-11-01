/* Small cases have 2 to 8 numbers, from a narrow range (so that equal
 * sums are common), a wider one, or a few values near +-10^9. Full-size
 * cases have 1000 numbers, in a shape the seed picks: strictly increasing,
 * so that all sums differ; random over the whole range, sometimes with
 * the first and the last sum made equal; values near +-10^9 of mixed
 * signs, sums near +-2 * 10^9 among them, with two equal sums planted
 * somewhere; or strictly increasing with two equal sums planted. */

/* Makes the sum at j equal to the sum at i, for j >= i + 2, by choosing
 * nums[j] and nums[j + 1] within the bounds. */
static void plant(int *nums, int i, int j)
{
    long long s = (long long)nums[i] + nums[i + 1];
    long long lo = s - 1000000000 > -1000000000 ? s - 1000000000 : -1000000000;
    long long hi = s + 1000000000 < 1000000000 ? s + 1000000000 : 1000000000;

    nums[j] = (int)rnd(lo, hi);
    nums[j + 1] = (int)(s - nums[j]);
}

static void increasing(int *nums, int n)
{
    int i;

    nums[0] = (int)rnd(-1000000000, -999999000);
    for (i = 1; i < n; i++)
        nums[i] = nums[i - 1] + (int)rnd(1, 2000000);
}

void gen(int small)
{
    static const int big[] = {1000000000, 999999999, 500000000, -500000000,
                              -999999999, -1000000000};
    int n = small ? (int)rnd(2, 8) : 1000, i, j, shape, top;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        shape = (int)rnd(0, 2);
        top = shape == 0 ? 4 : 30;
        for (i = 0; i < n; i++)
            nums[i] = shape == 2 ? big[rnd(0, 5)] : (int)rnd(-top, top);
        put_ints(nums, n);
        free(nums);
        return;
    }
    shape = (int)(gen_seed() % 4);
    if (shape == 0) {
        increasing(nums, n);
    } else if (shape == 1) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-1000000000, 1000000000);
        if (rnd_chance(50))
            plant(nums, 0, n - 2);
    } else {
        if (shape == 2) {
            for (i = 0; i < n; i++)
                nums[i] = rnd_chance(50) ? (int)rnd(700000000, 1000000000)
                                         : (int)rnd(-1000000000, -700000000);
        } else {
            increasing(nums, n);
        }
        i = (int)rnd(0, n - 4);
        j = (int)rnd(i + 2, n - 2);
        plant(nums, i, j);
    }
    put_ints(nums, n);
    free(nums);
}
