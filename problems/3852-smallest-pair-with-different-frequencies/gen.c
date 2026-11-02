/* Small cases have up to 8 entries. Some draw values from a narrow range,
 * so that they repeat. The rest give a few values, often spread out with
 * absent numbers between them, the same count, then often add or
 * remove one copy of one of them, so that [-1,-1] and a single odd one
 * out come up often. Full-size cases have up to 100 entries: random
 * values over nearly the whole range, or equal counts in which
 * the largest value, a random one or none differs. */
void gen(int small)
{
    int nums[101], vals[100] = {0};
    int maxn = small ? 8 : 100;
    int n = 0, i, j, k, f, d, lo, hi, step, pick, change;

    if (rnd_chance(40)) {
        n = small ? (int)rnd(1, 8) : 100;
        if (small) {
            lo = (int)rnd(1, 100);
            hi = lo + (int)rnd(0, 4);
            if (hi > 100)
                hi = 100;
        } else {
            lo = (int)rnd(1, 3);
            hi = (int)rnd(90, 100);
        }
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lo, hi);
        put_ints(nums, n);
        return;
    }

    f = small ? (int)rnd(1, 4) : (int)rnd(1, 10);
    d = small ? (int)rnd(2, maxn / f) : maxn / f;
    if (small) {
        step = (int)rnd(1, 3);
        lo = (int)rnd(1, 100 - (d - 1) * step);
        for (j = 0; j < d; j++)
            vals[j] = lo + j * step;
    } else {
        for (j = 0; j < 100; j++)
            vals[j] = j + 1;
        rnd_shuffle(vals, 100);
    }
    for (j = 0; j < d; j++)
        for (i = 0; i < f; i++)
            nums[n++] = vals[j];

    /* 0: leave it, 1: one copy more, 2: one copy fewer. */
    change = (int)rnd(0, 2);
    pick = (int)rnd(0, d - 1);
    if (!small && rnd_chance(50))
        for (j = 0; j < d; j++)
            if (vals[j] > vals[pick])
                pick = j;
    if (change == 1 && n < maxn) {
        nums[n++] = vals[pick];
    } else if (change == 2 && f > 1) {
        for (k = n - 1; nums[k] != vals[pick]; k--)
            ;
        nums[k] = nums[n - 1];
        n--;
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
}
