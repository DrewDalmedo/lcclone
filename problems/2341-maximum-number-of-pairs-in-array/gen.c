/* Small cases: 1 to 8 numbers from a run of at most 4 values, so that
 * counts of 2, 3 and more are common; the run sometimes starts at 0 or ends
 * at 100. Full-size cases have 99 or 100 numbers; the seed picks all equal,
 * a few values, all different (the first n of 0 to 100 shuffled), or random
 * in 0 to 100. */
void gen(int small)
{
    int nums[101];
    int n, span, base, shape, v, i;

    if (small) {
        n = (int)rnd(1, 8);
        span = (int)rnd(1, 4);
        shape = (int)rnd(0, 3);
        if (shape == 0)
            base = 0;
        else if (shape == 1)
            base = 100 - span + 1;
        else
            base = (int)rnd(0, 100 - span + 1);
        for (i = 0; i < n; i++)
            nums[i] = base + (int)rnd(0, span - 1);
        put_ints(nums, n);
        return;
    }
    n = rnd_chance(50) ? 100 : 99;
    shape = (int)(gen_seed() % 4);
    if (shape == 0) {
        v = (int)rnd(0, 100);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else if (shape == 1) {
        span = (int)rnd(2, 6);
        base = (int)rnd(0, 100 - span + 1);
        for (i = 0; i < n; i++)
            nums[i] = base + (int)rnd(0, span - 1);
    } else if (shape == 2) {
        for (i = 0; i <= 100; i++)
            nums[i] = i;
        rnd_shuffle(nums, 101);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(0, 100);
    }
    put_ints(nums, n);
}
