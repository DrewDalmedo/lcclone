/* Fills nums[n..total) with the values of vals in turn, each 2 or 3 times
 * (an odd one also just once, when oddOnce), and never leaves an even
 * value with a single copy. Returns the new length. */
static int fill(int *nums, int n, int total, const int *vals, int oddOnce)
{
    int j, t, k;

    for (j = 0; n < total; j++) {
        t = (int)rnd(oddOnce && vals[j] % 2 == 1 ? 1 : 2, 3);
        if (t > total - n)
            t = total - n;
        if (t == 1 && vals[j] % 2 == 0) {
            nums[n] = nums[n - 1];
            n++;
            continue;
        }
        for (k = 0; k < t; k++)
            nums[n++] = vals[j];
    }
    return n;
}

/* Small cases have up to 8 entries from a range of 7 numbers, so that even
 * and odd numbers, repeats and several unique even numbers all come up.
 * Full-size cases have 100 entries, with the seed picking the shape:
 * random numbers; every number repeated except odd ones and a single even
 * one at the very end; or no unique even number at all. */
void gen(int small)
{
    int nums[100], vals[100];
    int n = small ? (int)rnd(1, 8) : 100, lo, shape, i, j, u;

    if (small) {
        lo = (int)rnd(1, 94);
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lo, lo + 6);
        put_ints(nums, n);
        return;
    }
    shape = (int)rnd(0, 2);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 100);
        put_ints(nums, n);
        return;
    }
    for (i = 0; i < 100; i++)
        vals[i] = i + 1;
    rnd_shuffle(vals, 100);
    if (shape == 1) {
        for (j = 0; vals[j] % 2 != 0; j++)
            ;
        u = vals[j];
        vals[j] = vals[99];
        fill(nums, 0, 99, vals, rnd_chance(50));
        rnd_shuffle(nums, 99);
        nums[99] = u;
    } else {
        fill(nums, 0, 100, vals, 1);
        rnd_shuffle(nums, 100);
    }
    put_ints(nums, n);
}
