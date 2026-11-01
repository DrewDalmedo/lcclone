/* Mostly a strictly rising array that is then spoiled, so that the rising
 * prefix and suffix are long and the answer varies widely: a few values
 * changed, neighbours swapped, or a piece in the middle replaced by random
 * values. Sometimes a random array, a falling one or all one value, and
 * sometimes the rising array is left as it is.
 * Small cases have 1 to 8 values from a narrow range (just wide enough for
 * a rising array, 3 wider, or all of 1..50), so that equal values come up
 * often. Full-size ones have 50 values from 1..50; their rising array is
 * 1, 2, ..., 50, which gives the largest answer, 1275. */
void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(1, 8) : 50;
    int lo = 1, hi = 50, width, shape, need, v, i, a, b, k, t;

    if (small) {
        t = (int)rnd(0, 2);
        width = t == 0 ? n : t == 1 ? n + 3 : 50;
        lo = (int)rnd(1, 51 - width);
        hi = lo + width - 1;
    }
    shape = (int)rnd(0, 9);
    if (shape == 0) {
        if (small && rnd_chance(50)) {
            t = (int)rnd(0, 3);
            hi = lo + t > 50 ? 50 : lo + t;
        }
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lo, hi);
        put_ints(nums, n);
        return;
    }
    if (shape == 1 && rnd_chance(30)) {
        v = (int)rnd(lo, hi);
        for (i = 0; i < n; i++)
            nums[i] = v;
        put_ints(nums, n);
        return;
    }
    /* n different values from lo..hi in rising order: each value is taken
     * with the chance (values still needed) / (values left). */
    need = n;
    i = 0;
    for (v = lo; v <= hi && need > 0; v++)
        if (rnd(1, hi - v + 1) <= need) {
            nums[i++] = v;
            need--;
        }
    if (shape == 1) {
        /* Falling. */
        for (i = 0; i < n / 2; i++) {
            t = nums[i];
            nums[i] = nums[n - 1 - i];
            nums[n - 1 - i] = t;
        }
    } else if (shape >= 3 && shape <= 5) {
        /* 1 to 3 values changed. */
        for (k = shape - 2; k > 0; k--) {
            a = (int)rnd(0, n - 1);
            t = (int)rnd(lo, hi);
            nums[a] = t;
        }
    } else if ((shape == 6 || shape == 7) && n >= 2) {
        /* One or two pairs of neighbours swapped. */
        for (k = shape - 5; k > 0; k--) {
            a = (int)rnd(0, n - 2);
            t = nums[a];
            nums[a] = nums[a + 1];
            nums[a + 1] = t;
        }
    } else if (shape >= 8) {
        /* A piece replaced by random values. */
        a = (int)rnd(0, n - 1);
        b = (int)rnd(a, a + 3 < n - 1 ? a + 3 : n - 1);
        for (i = a; i <= b; i++)
            nums[i] = (int)rnd(lo, hi);
    }
    put_ints(nums, n);
}
