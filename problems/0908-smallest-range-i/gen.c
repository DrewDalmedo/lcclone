/* Small cases have 1 to 8 values. The seed picks:
 *   - values from 0 to 15 and k from 0 to 6, so that the gap is often just
 *     below, at, or just above 2k;
 *   - values from 9985 to 10000, k from 0 to 6;
 *   - values that are mostly 0 and 10^4, with k = 0, 4999, 5000, 5001 or
 *     10^4.
 * Full-size cases have 10^4 values from 0 to 10^4, both ends among them,
 * and the seed picks k: from 0 to 5000, 0 (the largest answer) or 10^4; the
 * largest or the smallest value may be put at the far end. */
void gen(int small)
{
    static const int ks[] = {0, 4999, 5000, 5001, 10000};
    int n, shape = small ? (int)rnd(0, 3) : (int)(gen_seed() % 4);
    int k, i, base, j, t, want_min;
    int *nums;

    n = small ? (int)rnd(1, 8) : 10000;
    nums = malloc((size_t)n * sizeof *nums);
    if (small && shape <= 2) {
        base = shape == 2 ? 9985 : 0;
        for (i = 0; i < n; i++)
            nums[i] = base + (int)rnd(0, 15);
        k = (int)rnd(0, 6);
    } else if (small) {
        for (i = 0; i < n; i++)
            if (rnd_chance(70))
                nums[i] = rnd_chance(50) ? 0 : 10000;
            else
                nums[i] = (int)rnd(0, 10000);
        k = ks[rnd(0, 4)];
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(0, 10000);
        nums[0] = 0;
        nums[1] = 10000;
        rnd_shuffle(nums, n);
        if (shape <= 1)
            k = (int)rnd(0, 5000);
        else
            k = shape == 2 ? 0 : 10000;
        if (rnd_chance(50)) {
            /* the smallest value last, or the largest */
            want_min = rnd_chance(50);
            j = 0;
            for (i = 1; i < n; i++)
                if (want_min ? nums[i] < nums[j] : nums[i] > nums[j])
                    j = i;
            t = nums[j];
            nums[j] = nums[n - 1];
            nums[n - 1] = t;
        }
    }
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
