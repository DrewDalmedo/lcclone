/* Increasing values built from the gaps between neighbours: a gap of 1
 * (the run goes on) about half the time, otherwise 2 or 3, or a wider
 * one. Sometimes one gap is at least 2^31, so the values jump from very
 * negative to positive (nums[i + 1] - nums[i] overflows an int there).
 * The whole stretch is then placed at INT_MIN, ending at INT_MAX, just
 * below zero (so a run crosses it), among small values, or anywhere.
 * Small cases have 0 to 8 values; full-size ones 20, all one run now
 * and then. */

#define TWO31 2147483648LL

void gen(int small)
{
    int n = small ? (int)rnd(0, 8) : 20, i;
    int nums[20];
    long long gap[20], span = 0, lo, hi, a;
    int one_run = !small && rnd_chance(20);
    long long wide = small ? 30 : 100000000;
    int where = (int)rnd(1, 100);

    for (i = 1; i < n; i++) {
        int r = (int)rnd(1, 100);

        if (one_run || r <= 50)
            gap[i] = 1;
        else if (r <= 75)
            gap[i] = rnd(2, 3);
        else
            gap[i] = rnd(4, wide);
        span += gap[i];
    }
    /* One huge gap: the other gaps total at most 19 * 10^8 < 2^31, so
     * there is room for it within the 2^32 - 1 the values can span. */
    if (n >= 2 && !one_run && rnd_chance(25)) {
        i = (int)rnd(1, n - 1);
        span -= gap[i];
        gap[i] = rnd(TWO31, 2 * TWO31 - 1 - span);
        span += gap[i];
    }
    lo = -TWO31;
    hi = TWO31 - 1 - span;
    if (where <= 15)
        a = lo;
    else if (where <= 30)
        a = hi;
    else if (where <= 55)
        a = -rnd(1, span + 1);
    else if (where <= 80)
        a = rnd(-10, 10);
    else
        a = rnd(lo, hi);
    if (a < lo)
        a = lo;
    if (a > hi)
        a = hi;
    for (i = 0; i < n; i++) {
        if (i > 0)
            a += gap[i];
        nums[i] = (int)a;
    }
    put_ints(nums, n);
}
