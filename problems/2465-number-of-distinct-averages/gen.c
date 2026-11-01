/* An even number of values, shuffled. The seed picks a shape: plain random
 * values; values that mirror around some centre, so every pair has the same
 * sum (the answer is 1); half the values equal to some c and the other half
 * distinct and at least c, so every pair sum differs (the answer is n / 2);
 * or values from three neighbours x, x + 1, x + 2, so the pair sums are
 * consecutive and averaging with integer division merges them. Small cases
 * have 2 to 8 values, usually from a narrow range; full-size ones have 100
 * values from 0 to 100. */
void gen(int small)
{
    int n = small ? 2 * (int)rnd(1, 4) : 100, half = n / 2, i, j, t;
    int top = small && !rnd_chance(20) ? (int)rnd(3, 10) : 100;
    int shape = (int)rnd(0, 3);
    int nums[100], pool[101];

    if (shape == 1) {
        int s = (int)rnd(0, 2 * top);
        int lo = s > top ? s - top : 0, hi = s < top ? s : top;

        for (i = 0; i < half; i++) {
            nums[2 * i] = (int)rnd(lo, hi);
            nums[2 * i + 1] = s - nums[2 * i];
        }
    } else if (shape == 2) {
        int c = (int)rnd(0, top - half + 1), m = top - c + 1;

        for (i = 0; i < m; i++)
            pool[i] = c + i;
        for (i = 0; i < half; i++) {
            j = (int)rnd(i, m - 1);
            t = pool[i];
            pool[i] = pool[j];
            pool[j] = t;
            nums[i] = c;
            nums[half + i] = pool[i];
        }
    } else if (shape == 3) {
        int x = (int)rnd(0, top - 2);

        for (i = 0; i < n; i++)
            nums[i] = x + (int)rnd(0, 2);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(0, top);
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
}
