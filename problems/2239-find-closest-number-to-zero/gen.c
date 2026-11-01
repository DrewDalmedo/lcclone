/* A random array. Small cases have a few values near zero, so that d and
 * -d and repeats come up often (half of them have no zero); now and then
 * every value is -10^5 or every value is 10^5. Full-size ones have 1000
 * values, by a shape the seed picks: random values; values far from zero,
 * with the nearest pair d and -d placed at the two ends in either order;
 * or every value -10^5 (or 10^5). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000, i, top, shape, zero, d;
    int first, last;
    int *nums = malloc((size_t)n * sizeof *nums);

    shape = small ? (rnd_chance(90) ? 0 : 2) : (int)rnd(0, 2);
    top = small ? (int)rnd(2, 9) : 100000;
    zero = rnd_chance(50);
    for (i = 0; i < n; i++) {
        d = (int)rnd(-top, top);
        nums[i] = d == 0 && !zero ? top : d;
    }
    if (shape == 1) {
        d = (int)rnd(1, 50000);
        for (i = 0; i < n; i++) {
            nums[i] = (int)rnd(d + 1, 100000);
            if (rnd_chance(50))
                nums[i] = -nums[i];
        }
        first = rnd_chance(50) ? d : -d;
        last = -first;
        nums[0] = first;
        nums[n - 1] = last;
    } else if (shape == 2) {
        d = rnd_chance(50) ? 100000 : -100000;
        for (i = 0; i < n; i++)
            nums[i] = d;
    }
    put_ints(nums, n);
    free(nums);
}
