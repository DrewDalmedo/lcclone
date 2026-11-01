/* Values come in runs (a value often repeats the one before it) with zeros
 * mixed in. The seed picks a shape: a plain mix, long runs, many zeros
 * spread out (the slowest case for moving zeros one place at a time), or
 * only 999, 1000 and zeros, so that 1000 + 1000 merges come up. Small cases
 * have 2 to 8 values, mostly from 0 to 3; full-size ones have 2000. */
static int value(int small, int shape)
{
    if (shape == 3)
        return rnd_chance(50) ? 1000 : 999;
    return (int)rnd(1, small ? 3 : 1000);
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 2000, i;
    int shape = (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (i > 0 && rnd_chance(shape == 1 ? 70 : 35))
            nums[i] = nums[i - 1];
        else if (rnd_chance(shape == 2 ? 50 : 20))
            nums[i] = 0;
        else
            nums[i] = value(small, shape);
    }
    put_ints(nums, n);
    free(nums);
}
