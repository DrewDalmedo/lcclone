/* Small cases: 0 to 8 values, mostly from -3 to 3 so that ties and zeros
 * are common, now and then -10^9 or 10^9. Full-size ones have 10^5 values
 * in a shape the seed picks: different values ascending, descending or in
 * random order (from -10^9 to 10^9, both included), all equal, or random
 * values with many repeats. */
void gen(int small)
{
    int n = small ? (int)rnd(0, 8) : 100000, shape, i, v;
    int *a = malloc(((size_t)n + 1) * sizeof *a);

    if (small) {
        for (i = 0; i < n; i++) {
            if (rnd_chance(15))
                v = rnd_chance(50) ? -1000000000 : 1000000000;
            else
                v = (int)rnd(-3, 3);
            a[i] = v;
        }
    } else {
        shape = (int)rnd(0, 4);
        if (shape == 3) {
            v = (int)rnd(-1000000000, 1000000000);
            for (i = 0; i < n; i++)
                a[i] = v;
        } else if (shape == 4) {
            for (i = 0; i < n; i++)
                a[i] = (int)rnd(-1000, 1000) * 1000000;
        } else {
            /* Different values: one from each block of 20000. */
            for (i = 0; i < n; i++)
                a[i] = (int)(-1000000000LL + 20000LL * i + rnd(0, 19999));
            a[0] = -1000000000;
            a[n - 1] = 1000000000;
            if (shape == 1)
                for (i = 0; i < n / 2; i++) {
                    v = a[i];
                    a[i] = a[n - 1 - i];
                    a[n - 1 - i] = v;
                }
            else if (shape == 2)
                rnd_shuffle(a, n);
        }
    }
    put_ints(a, n);
    free(a);
}
