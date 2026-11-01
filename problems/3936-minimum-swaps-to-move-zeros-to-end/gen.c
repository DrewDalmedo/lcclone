/* The shape is picked at random (by the seed for full-size cases): zeros
 * scattered with a random share; a block of zeros in front of the non-zero
 * values; one early zero, a long run of non-zero values and a few zeros
 * already at the back; or zeros and non-zero values taking turns. Small
 * cases have up to 8 values, mostly 1 to 3 so values repeat, now and then
 * up to 100; full-size ones have 100 values from 1 to 100. */
static int value(int small)
{
    int top;

    if (!small)
        return (int)rnd(1, 100);
    top = rnd_chance(15) ? 100 : 3;
    return (int)rnd(1, top);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, nums[100], i, shape, share;
    int lead, tail, first;

    shape = small ? (int)rnd(0, 3) : (int)(gen_seed() % 4);
    for (i = 0; i < n; i++)
        nums[i] = value(small);
    if (shape == 0) {
        share = (int)rnd(0, 100);
        for (i = 0; i < n; i++)
            if (rnd_chance(share))
                nums[i] = 0;
    } else if (shape == 1) {
        lead = small ? (int)rnd(0, n) : (int)rnd(n / 2 - 5, n / 2 + 5);
        for (i = 0; i < lead; i++)
            nums[i] = 0;
    } else if (shape == 2) {
        tail = (int)rnd(0, n - 1);
        nums[0] = 0;
        for (i = n - tail; i < n; i++)
            nums[i] = 0;
    } else {
        first = (int)rnd(0, 1);
        for (i = first; i < n; i += 2)
            nums[i] = 0;
    }
    put_ints(nums, n);
}
