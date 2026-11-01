/* An array of values from 0 to 10^5. Small cases hold 1 to 8 values from
 * a tiny pool, so that 0, ties and all-odd arrays come up often; the pool
 * may include 10^5 and 99999. Full-size cases hold 1000 to 2000 values
 * (often exactly 2000), in one of these shapes, picked by the seed:
 *   0: values drawn from a pool of 2 to 60 random values;
 *   1: all different, so every count is 1;
 *   2: one even value 256 to 1000 times, an odd value even more often,
 *      and the rest from a pool;
 *   3: all odd;
 *   4: 2 to 5 even values tied on the same count, among different odd
 *      values. */
static int pick_odd(void)
{
    return 2 * (int)rnd(0, 49999) + 1;
}

static void small_case(void)
{
    static const int edgy[7] = {0, 2, 4, 1, 99999, 99998, 100000};
    static const int odd[5] = {1, 3, 5, 7, 99999};
    int nums[8], n = (int)rnd(1, 8), shape = (int)rnd(0, 3), i;

    for (i = 0; i < n; i++) {
        if (shape == 0)
            nums[i] = (int)rnd(0, 6);
        else if (shape == 1)
            nums[i] = odd[rnd(0, 4)];
        else if (shape == 2)
            nums[i] = edgy[rnd(0, 6)];
        else
            nums[i] = (int)rnd(0, 20);
    }
    put_ints(nums, n);
}

void gen(int small)
{
    int pool[60], n, shape, k, i, v, c, t, tied[5];
    int *nums;
    char *seen;

    if (small) {
        small_case();
        return;
    }
    n = rnd_chance(50) ? 2000 : (int)rnd(1000, 2000);
    shape = (int)rnd(0, 4);
    nums = malloc((size_t)n * sizeof *nums);
    k = (int)rnd(2, 60);
    for (i = 0; i < k; i++)
        pool[i] = (int)rnd(0, 100000);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = pool[rnd(0, k - 1)];
    } else if (shape == 1) {
        seen = calloc(100001, 1);
        for (i = 0; i < n; i++) {
            do
                v = (int)rnd(0, 100000);
            while (seen[v]);
            seen[v] = 1;
            nums[i] = v;
        }
        free(seen);
    } else if (shape == 2) {
        v = 2 * (int)rnd(0, 50000);
        c = (int)rnd(256, 1000);
        for (i = 0; i < c; i++)
            nums[i] = v;
        v = pick_odd();
        t = c + (int)rnd(1, 50);
        for (; i < c + t && i < n; i++)
            nums[i] = v;
        for (; i < n; i++)
            nums[i] = pool[rnd(0, k - 1)];
        rnd_shuffle(nums, n);
    } else if (shape == 3) {
        for (i = 0; i < n; i++)
            nums[i] = pick_odd();
    } else {
        t = (int)rnd(2, 5);
        c = (int)rnd(1, n / t);
        for (i = 0; i < t; i++) {
            tied[i] = 2 * (int)rnd(0, 50000);
            for (k = 0; k < i; k++)
                if (tied[k] == tied[i])
                    break;
            if (k < i)
                i--;            /* the same value twice: draw again */
        }
        for (i = 0; i < t * c; i++)
            nums[i] = tied[i / c];
        for (; i < n; i++)
            nums[i] = pick_odd();
        rnd_shuffle(nums, n);
    }
    put_ints(nums, n);
    free(nums);
}
