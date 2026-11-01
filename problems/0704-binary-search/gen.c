/* nums is drawn by selection sampling: each value of a range is taken
 * with the chance that leaves exactly n values in the end, so they come
 * out distinct and in increasing order. Small cases take up to 8 values
 * from [-12, 12], or now and then from the top or bottom 14 values
 * allowed, and target is an element half of the time, else a value in or
 * just around that range. Full-size cases take 10^4 of the 19999 values
 * allowed, and the seed picks target: absent and above every element (the
 * slowest for a scan from the front), the last element, absent and below
 * every element, absent and inside a gap, or a random element. */
static int pick(int *nums, int n, int lo, int hi)
{
    int need = n, left = hi - lo + 1, k = 0, v;

    for (v = lo; v <= hi && need > 0; v++, left--)
        if (rnd(0, left - 1) < need) {
            nums[k++] = v;
            need--;
        }
    return k;
}

void gen(int small)
{
    static int nums[10000];
    static unsigned char taken[19999];
    int n, lo, hi, shape, target, i, v;

    if (small) {
        n = (int)rnd(1, 8);
        shape = (int)rnd(0, 5);
        if (shape == 0) {
            lo = -9999;
            hi = -9986;
        } else if (shape == 1) {
            lo = 9986;
            hi = 9999;
        } else {
            lo = -12;
            hi = 12;
        }
        pick(nums, n, lo, hi);
        if (rnd_chance(50)) {
            i = (int)rnd(0, n - 1);
            target = nums[i];
        } else {
            target = (int)rnd(lo - 2, hi + 2);
            if (target < -9999)
                target = -9999;
            if (target > 9999)
                target = 9999;
        }
        put_ints(nums, n);
        put_int(target);
        return;
    }

    n = 10000;
    shape = (int)(gen_seed() % 5);
    lo = shape == 2 ? -9998 : -9999;
    hi = shape == 0 ? 9998 : 9999;
    pick(nums, n, lo, hi);
    if (shape == 0) {
        target = 9999;
    } else if (shape == 1) {
        target = nums[n - 1];
    } else if (shape == 2) {
        target = -9999;
    } else if (shape == 3 && nums[n - 1] - nums[0] + 1 == n) {
        /* No gap inside (all but impossible): just past an end. */
        target = nums[n - 1] < 9999 ? nums[n - 1] + 1 : nums[0] - 1;
    } else if (shape == 3) {
        memset(taken, 0, sizeof taken);
        for (i = 0; i < n; i++)
            taken[nums[i] + 9999] = 1;
        do
            v = (int)rnd(nums[0] + 1, nums[n - 1] - 1);
        while (taken[v + 9999]);
        target = v;
    } else {
        i = (int)rnd(0, n - 1);
        target = nums[i];
    }
    put_ints(nums, n);
    put_int(target);
}
