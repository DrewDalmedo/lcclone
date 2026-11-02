/* Random heights. Small: 2 to 8 checkpoints with heights -5 to 5, or now
 * and then over the whole range, sometimes all equal. Full size: 100
 * checkpoints; the seed picks random heights over the whole range, a
 * gentle slope whose largest step is the wrap from the last checkpoint
 * back to the first, or all heights equal. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 100, i, shape, top, v, step;
    int nums[100];

    shape = small ? (rnd_chance(10) ? 2 : 0) : (int)(gen_seed() % 3);
    top = small && rnd_chance(75) ? 5 : 100;
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-top, top);
    } else if (shape == 1) {
        /* Climb from near -100 to near 100 in steps of 0 to 3, then fall
         * back in one step across the wrap. */
        v = (int)rnd(-100, -90);
        for (i = 0; i < n; i++) {
            nums[i] = v;
            step = (int)rnd(0, 3);
            if (v + step <= 100)
                v += step;
        }
        if (rnd_chance(50))
            for (i = 0; i < n; i++)
                nums[i] = -nums[i];
    } else {
        v = (int)rnd(-top, top);
        for (i = 0; i < n; i++)
            nums[i] = v;
    }
    put_ints(nums, n);
}
