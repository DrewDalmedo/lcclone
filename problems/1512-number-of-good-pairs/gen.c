/* Small cases: up to 8 values from a run of at most 4 numbers, so that
 * repeats are common; the run sometimes starts at 1 or ends at 100.
 * Full-size cases have 100 values; the seed picks all equal, from a few
 * numbers, all different (a shuffle of 1..100), or random in 1..100. */
void gen(int small)
{
    int nums[100];
    int n, span, base, shape, v, i;

    if (small) {
        n = (int)rnd(1, 8);
        span = (int)rnd(1, 4);
        shape = (int)rnd(0, 3);
        if (shape == 0)
            base = 1;
        else if (shape == 1)
            base = 100 - span + 1;
        else
            base = (int)rnd(1, 100 - span + 1);
        for (i = 0; i < n; i++)
            nums[i] = base + (int)rnd(0, span - 1);
        put_ints(nums, n);
        return;
    }
    n = 100;
    shape = (int)(gen_seed() % 4);
    if (shape == 0) {
        v = (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else if (shape == 1) {
        span = (int)rnd(2, 6);
        base = (int)rnd(1, 100 - span + 1);
        for (i = 0; i < n; i++)
            nums[i] = base + (int)rnd(0, span - 1);
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 100);
    }
    put_ints(nums, n);
}
