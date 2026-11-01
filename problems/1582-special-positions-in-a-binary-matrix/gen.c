/* A random grid of 0s and 1s. Half the time each cell is 1 with a random
 * chance; otherwise some 1s go in distinct rows and distinct columns (so
 * each starts out special) and a few extra 1s may spoil some of them.
 * Small grids are 1 to 5 on a side; full-size ones are 100 x 100, 1 x 100,
 * 100 x 1 or of random sides. */
void gen(int small)
{
    int m, n, shape, k, extra, i, j, t, p;
    int *a, *rows, *cols;

    if (small) {
        m = (int)rnd(1, 5);
        n = (int)rnd(1, 5);
    } else {
        shape = (int)rnd(0, 3);
        m = shape == 1 ? 1 : 100;
        n = shape == 2 ? 1 : 100;
        if (shape == 3) {
            m = (int)rnd(1, 100);
            n = (int)rnd(1, 100);
        }
    }
    a = calloc((size_t)m * n, sizeof *a);
    rows = malloc((size_t)m * sizeof *rows);
    cols = malloc((size_t)n * sizeof *cols);
    if (rnd_chance(50)) {
        p = small ? (int)rnd(10, 70) : (int)rnd(1, 20);
        for (i = 0; i < m * n; i++)
            a[i] = rnd_chance(p);
    } else {
        for (i = 0; i < m; i++)
            rows[i] = i;
        for (j = 0; j < n; j++)
            cols[j] = j;
        rnd_shuffle(rows, m);
        rnd_shuffle(cols, n);
        k = (int)rnd(0, m < n ? m : n);
        for (t = 0; t < k; t++)
            a[rows[t] * n + cols[t]] = 1;
        extra = (int)rnd(0, small ? 2 : 3);
        for (t = 0; t < extra; t++) {
            i = (int)rnd(0, m - 1);
            j = (int)rnd(0, n - 1);
            a[i * n + j] = 1;
        }
    }
    put_int_grid(a, m, n);
    free(a);
    free(rows);
    free(cols);
}
