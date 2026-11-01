/* The seed picks a shape: nothing is a multiple of 6 (the answer is 0),
 * only multiples of 6, or a mix in which multiples of 6 are common. Small
 * cases have up to 8 values, usually at most 40 so that repeats are
 * frequent, sometimes up to 1000; full-size ones have 1000 values up to
 * 1000, and one shape is every value at 996, the largest multiple of 6. */
static int pick(int shape, int top)
{
    int v;

    if (shape == 0) {
        do
            v = (int)rnd(1, top);
        while (v % 6 == 0);
        return v;
    }
    if (shape == 1 || rnd_chance(40))
        return 6 * (int)rnd(1, top / 6);
    return (int)rnd(1, top);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000, i;
    int top = small && !rnd_chance(25) ? 40 : 1000;
    int shape = (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = !small && shape == 3 ? 996 : pick(shape, top);
    put_ints(nums, n);
    free(nums);
}
