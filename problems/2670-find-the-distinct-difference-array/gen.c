/* Random values; the seed picks how many different ones can occur, so
 * that repeats are common in some cases and rare in others. Small cases
 * have up to 8 elements; full-size ones have 50, sometimes every value
 * from 1 to 50 once (in a random order), sometimes values near 50. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50;
    int shape = (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);
    int i, lo = 1, hi;

    if (shape == 0)
        hi = 3;
    else if (shape == 1)
        hi = small ? 8 : 20;
    else
        hi = 50;
    if (shape == 2 && !small) {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    } else {
        if (shape == 3)
            lo = 45;
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lo, hi);
    }
    put_ints(nums, n);
    free(nums);
}
