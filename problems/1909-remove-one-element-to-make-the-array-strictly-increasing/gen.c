/* Most cases are a strictly increasing run of n - 1 values with one more
 * value put in at a random place (so the answer is true), which is then
 * often spoiled: one value replaced, or two neighbours swapped. A few are
 * random arrays. Small cases have 2 to 8 values from a narrow range (just
 * wide enough for the run, 2 wider, or 20 wide; at the bottom, the top or
 * anywhere in 1..1000), so that equal values come up often. Full-size ones
 * have 1000 values from 1..1000, the extra value often near the end. */
void gen(int small)
{
    int nums[1000];
    int n = small ? (int)rnd(2, 8) : 1000;
    int m = n - 1, width, lo, hi, need, v, i, pos, x, a, t, r, spoil;

    if (small) {
        r = (int)rnd(0, 2);
        width = r == 0 ? m : r == 1 ? m + 2 : 20;
        r = (int)rnd(0, 2);
        lo = r == 0 ? 1 : r == 1 ? 1001 - width : (int)rnd(1, 1001 - width);
    } else {
        width = 1000;
        lo = 1;
    }
    hi = lo + width - 1;
    if (rnd_chance(10)) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lo, hi);
        put_ints(nums, n);
        return;
    }
    /* m different values from lo..hi in increasing order: each value is
     * taken with the chance (values still needed) / (values left). */
    need = m;
    i = 0;
    for (v = lo; v <= hi && need > 0; v++)
        if (rnd(1, hi - v + 1) <= need) {
            nums[i++] = v;
            need--;
        }
    if (!small && rnd_chance(50))
        pos = (int)rnd(m - 10, m);
    else
        pos = (int)rnd(0, m);
    x = (int)rnd(lo, hi);
    for (i = m; i > pos; i--)
        nums[i] = nums[i - 1];
    nums[pos] = x;
    spoil = (int)rnd(0, 3);
    if (spoil == 1) {
        a = (int)rnd(0, n - 1);
        x = (int)rnd(lo, hi);
        nums[a] = x;
    } else if (spoil == 2) {
        a = (int)rnd(0, n - 2);
        t = nums[a];
        nums[a] = nums[a + 1];
        nums[a + 1] = t;
    }
    put_ints(nums, n);
}
