/* Small cases are up to 8 values from a small range, so that repeats and
 * fewer than three different values are common; some draw from the ends
 * of the int range instead. Full-size ones have 10^4 values, in a shape the
 * seed picks: all different and spread over the whole int range, shuffled
 * (so deduplicating by scanning a growing list is slowest); the same in
 * increasing order; only three different values, with a single INT_MIN as
 * the answer; or only two different values. */
#define SPREAD 429496

void gen(int small)
{
    static const int ends[] = {INT_MIN, INT_MIN + 1, -1, 0, 1,
                               INT_MAX - 1, INT_MAX};
    static int nums[10000];
    int n, i, shape, a, b, lo;

    if (small) {
        n = (int)rnd(1, 8);
        shape = (int)rnd(0, 3);
        lo = shape == 0 ? 1 : shape == 1 ? 3 : 10;
        for (i = 0; i < n; i++) {
            if (shape == 3) {
                a = ends[rnd(0, 6)];
                nums[i] = a;
            } else {
                nums[i] = (int)rnd(-lo, lo);
            }
        }
    } else {
        n = 10000;
        shape = (int)(gen_seed() % 4);
        if (shape <= 1) {
            /* Value i lies in [INT_MIN + i * SPREAD, INT_MIN + (i + 1) *
             * SPREAD), and 10^4 * SPREAD < 2^32. */
            for (i = 0; i < n; i++) {
                a = (int)rnd(0, SPREAD - 1);
                nums[i] = (int)(INT_MIN + (long long)i * SPREAD + a);
            }
            if (shape == 0)
                rnd_shuffle(nums, n);
        } else {
            a = (int)rnd(INT_MIN + 1, INT_MAX);
            do
                b = (int)rnd(INT_MIN + 1, INT_MAX);
            while (b == a);
            for (i = 0; i < n; i++)
                nums[i] = rnd_chance(50) ? a : b;
            if (shape == 2)
                nums[rnd(0, n - 1)] = INT_MIN;
        }
    }
    put_ints(nums, n);
}
