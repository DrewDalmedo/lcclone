/* Small cases: up to 8 values from -5 to 5, so that zero totals, ties and
 * "no positive window" come up often. Full-size cases have 100 values; the
 * seed picks the shape: random values with random l and r; values near
 * 1000 with long windows (answers near 10^5); mostly negative values with
 * every length allowed; or values that cancel out in pairs. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, l, r, v;
    int shape = small ? 0 : (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (small)
            v = (int)rnd(-5, 5);
        else if (shape == 0)
            v = (int)rnd(-1000, 1000);
        else if (shape == 1)
            v = (int)rnd(900, 1000);
        else if (shape == 2)
            v = rnd_chance(10) ? (int)rnd(1, 1000) : (int)rnd(-1000, 0);
        else
            v = (i % 2 == 0) ? (int)rnd(1, 3) * 300 : -nums[i - 1];
        nums[i] = v;
    }
    if (shape == 1) {
        l = (int)rnd(90, n);
        r = (int)rnd(l, n);
    } else if (shape == 2) {
        l = 1;
        r = n;
    } else {
        l = (int)rnd(1, n);
        r = (int)rnd(l, n);
    }
    put_ints(nums, n);
    put_int(l);
    put_int(r);
    free(nums);
}
