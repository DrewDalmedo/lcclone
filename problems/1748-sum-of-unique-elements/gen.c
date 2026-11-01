/* Small cases have up to 10 entries from a narrow range (1..3, 1..8 or
 * 95..100) or the whole range, so that values appear once, twice, three
 * times and more. Full-size ones have 100 entries, with the seed picking
 * the shape: every value from 1 to 100 once (the largest sum), random
 * values, a mix of values appearing 1, 2 or 3 times, or values that all
 * appear twice (a sum of 0). */
void gen(int small)
{
    static const int lows[] = {1, 1, 95, 1}, highs[] = {3, 8, 100, 100};
    int nums[100];
    int n, i, j, shape, v, times;

    if (small) {
        shape = (int)rnd(0, 3);
        n = (int)rnd(1, 10);
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lows[shape], highs[shape]);
    } else {
        shape = (int)rnd(0, 3);
        n = 100;
        if (shape == 0) {
            for (i = 0; i < n; i++)
                nums[i] = i + 1;
        } else if (shape == 1) {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(1, 100);
        } else {
            int vals[100];

            for (i = 0; i < 100; i++)
                vals[i] = i + 1;
            rnd_shuffle(vals, 100);
            for (i = 0, v = 0; i < n; v++) {
                times = shape == 2 ? (int)rnd(1, 3) : 2;
                for (j = 0; j < times && i < n; j++)
                    nums[i++] = vals[v];
            }
        }
        rnd_shuffle(nums, n);
    }
    put_ints(nums, n);
}
