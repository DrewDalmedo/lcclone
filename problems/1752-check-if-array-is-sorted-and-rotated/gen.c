/* Most cases are a sorted array cut at a random point (true), which is
 * then often spoiled: two values swapped, or one value replaced, which
 * usually makes the answer false. Small cases have up to 8 values from
 * 1..3, 1..9, 95..100 or 1..100, so that equal values and equal values
 * around the cut come up often. Full-size ones have 100 values from
 * 1..100, the seed picking the range (1..100 or 1..10), the cut and the
 * spoiling; a few are random arrays. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    static const int lows[] = {1, 1, 95, 1}, highs[] = {3, 9, 100, 100};
    int sorted[100], nums[100];
    int n, lo, hi, cut, i, a, b, t, spoil, r;

    if (small) {
        r = (int)rnd(0, 3);
        n = (int)rnd(1, 8);
        lo = lows[r];
        hi = highs[r];
    } else {
        n = 100;
        lo = 1;
        hi = rnd_chance(30) ? 10 : 100;
    }
    for (i = 0; i < n; i++)
        sorted[i] = (int)rnd(lo, hi);
    if (!small && rnd_chance(15)) {
        put_ints(sorted, n);
        return;
    }
    qsort(sorted, (size_t)n, sizeof *sorted, cmp_int);
    cut = (int)rnd(0, n - 1);
    for (i = 0; i < n; i++)
        nums[i] = sorted[(cut + i) % n];
    spoil = (int)rnd(0, 3);
    if (spoil == 1 && n >= 2) {
        a = (int)rnd(0, n - 1);
        b = (int)rnd(0, n - 2);
        if (b >= a)
            b++;
        t = nums[a];
        nums[a] = nums[b];
        nums[b] = t;
    } else if (spoil == 2) {
        a = (int)rnd(0, n - 1);
        nums[a] = (int)rnd(lo, hi);
    }
    put_ints(nums, n);
}
