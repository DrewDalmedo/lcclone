/* Random values and a random k. Small cases have up to 8 values from a
 * narrow range (often 1..4), so that copies of one value straddle the
 * cut; sometimes the range is pinned to the bounds 1 and 100. Full-size
 * cases have 100 values; the seed may make them half 1s and half 100s,
 * the shape of the largest answer. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, k, lo, hi, shape, v;
    int nums[100];

    shape = (int)rnd(0, 3);
    if (small) {
        if (shape == 0) {
            lo = 1;
            hi = 4;
        } else if (shape == 1) {
            lo = 97;
            hi = 100;
        } else {
            lo = 1;
            hi = 100;
        }
    } else {
        lo = 1;
        hi = shape == 0 ? 3 : 100;
    }
    for (i = 0; i < n; i++) {
        if (!small && shape == 1) {
            v = (int)rnd(0, 1);
            nums[i] = v ? 100 : 1;
        } else {
            nums[i] = (int)rnd(lo, hi);
        }
    }
    if (!small && shape == 1)
        k = (int)rnd(40, 60);
    else
        k = (int)rnd(1, n);
    put_ints(nums, n);
    put_int(k);
}
