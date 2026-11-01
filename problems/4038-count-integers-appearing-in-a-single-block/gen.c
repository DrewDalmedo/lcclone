/* An array of values in 1..100. The seed picks the shape: entries drawn
 * from a few values, so that most values come back after others; or the
 * array built from blocks of equal entries, whose values may or may not
 * come back in a later block. Small cases have up to 8 entries and up to
 * 4 values, sometimes the top ones (97..100). Full-size ones have 100
 * entries; there the seed also picks values from all of 1..100, or every
 * value 1..100 once, in a random order. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? (int)rnd(0, 1) : (int)rnd(0, 3);
    int k = small ? (int)rnd(2, 4) : (int)rnd(2, 12);
    int base = 1, i, j, v, len;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small && rnd_chance(10))
        k = 1;
    if (rnd_chance(25))
        base = 101 - k;
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = base + (int)rnd(0, k - 1);
    } else if (shape == 1) {
        if (!small && rnd_chance(50))
            k = (int)rnd(13, 100);
        if (base > 1)
            base = 101 - k;
        for (i = 0; i < n; i += len) {
            v = base + (int)rnd(0, k - 1);
            len = (int)rnd(1, small ? 3 : 6);
            for (j = i; j < i + len && j < n; j++)
                nums[j] = v;
        }
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 100);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    }
    put_ints(nums, n);
    free(nums);
}
