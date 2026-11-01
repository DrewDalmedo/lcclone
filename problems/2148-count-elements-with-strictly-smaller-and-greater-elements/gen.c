/* An array of values. Small cases have up to 8 values from a narrow
 * range, so copies of the minimum and the maximum are common; the range
 * is sometimes pushed against -10^5 or 10^5, and sometimes a single
 * value. Full-size ones have 100 values in a shape picked by the seed:
 * the full range, all positive, all negative, or a few distinct values
 * including both extremes. */
void gen(int small)
{
    int nums[100], n, lo, hi, shape, pick, i;

    if (small) {
        n = (int)rnd(1, 8);
        shape = (int)rnd(0, 5);
        if (shape == 3) {
            lo = 99996;
            hi = 100000;
        } else if (shape == 4) {
            lo = -100000;
            hi = -99996;
        } else {
            lo = (int)rnd(-4, 2);
            hi = shape == 5 ? lo : lo + (int)rnd(2, 5);
        }
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lo, hi);
        put_ints(nums, n);
        return;
    }
    n = 100;
    shape = (int)rnd(0, 3);
    lo = shape == 1 ? 1 : -100000;
    hi = shape == 2 ? -1 : 100000;
    for (i = 0; i < n; i++) {
        if (shape == 3) {
            pick = (int)rnd(0, 3);
            nums[i] = pick == 0 ? -100000 : pick == 1 ? 100000 : pick == 2 ? 0 : 7;
        } else {
            nums[i] = (int)rnd(lo, hi);
        }
    }
    put_ints(nums, n);
}
