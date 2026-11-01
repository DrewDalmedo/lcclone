/* Small cases are up to 8 values from a tiny range, so that ties for the
 * degree come up often; some use only the two ends of the range, 0 and
 * 49999, and some values from the whole range, mostly all different.
 * Full-size cases have 5 * 10^4 elements, in a shape the seed picks: every
 * value from 0 to 49999 once, shuffled (degree 1, with every value tied);
 * 25000 values twice each, shuffled; one value at both ends and all
 * different values between them (the answer is the whole array); or
 * values from a window of 1000, so that many values tie at about 50
 * copies. */
void gen(int small)
{
    static int nums[50000], pool[50000];
    int n, i, kind, v, base;

    if (small) {
        n = (int)rnd(1, 8);
        kind = (int)rnd(0, 3);
        for (i = 0; i < n; i++) {
            if (kind == 0)
                v = (int)rnd(0, 2);
            else if (kind == 1)
                v = (int)rnd(0, 4);
            else if (kind == 2)
                v = rnd_chance(50) ? 0 : 49999;
            else
                v = (int)rnd(0, 49999);
            nums[i] = v;
        }
        put_ints(nums, n);
        return;
    }

    n = 50000;
    kind = (int)(gen_seed() % 4);
    for (i = 0; i < 50000; i++)
        pool[i] = i;
    rnd_shuffle(pool, 50000);
    if (kind == 0) {
        for (i = 0; i < n; i++)
            nums[i] = pool[i];
    } else if (kind == 1) {
        for (i = 0; i < n; i++)
            nums[i] = pool[i / 2];
        rnd_shuffle(nums, n);
    } else if (kind == 2) {
        nums[0] = pool[0];
        nums[n - 1] = pool[0];
        for (i = 1; i < n - 1; i++)
            nums[i] = pool[i];
    } else {
        base = (int)rnd(0, 49000);
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, 999);
            nums[i] = base + v;
        }
    }
    put_ints(nums, n);
}
