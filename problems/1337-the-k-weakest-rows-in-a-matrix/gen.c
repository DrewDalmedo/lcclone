/* A grid of rows that are 1s then 0s, and k. Small grids have 2 to 6 rows
 * of 2 to 5 cells; full-size ones are 100 x 100, 100 x 2 or 2 x 100. The
 * seed picks the strengths: random (from a narrow range when small, so
 * that ties are common), a few values only (many ties), rising or falling
 * down the rows, or all equal. k is m a third of the time. */
void gen(int small)
{
    int m, n, shape, hi, i, j, s, v;
    int *strength, *a;

    if (small) {
        m = (int)rnd(2, 6);
        n = (int)rnd(2, 5);
    } else {
        shape = (int)rnd(0, 2);
        m = shape == 2 ? 2 : 100;
        n = shape == 1 ? 2 : 100;
    }
    strength = malloc((size_t)m * sizeof *strength);
    a = malloc((size_t)m * n * sizeof *a);
    shape = (int)rnd(0, 4);
    if (shape == 0) {
        hi = small ? (int)rnd(0, n) : n;
        for (i = 0; i < m; i++)
            strength[i] = (int)rnd(0, hi);
    } else if (shape == 1) { /* a few values only */
        int few[3];

        for (j = 0; j < 3; j++)
            few[j] = (int)rnd(0, n);
        for (i = 0; i < m; i++) {
            j = (int)rnd(0, 2);
            strength[i] = few[j];
        }
    } else if (shape == 2 || shape == 3) { /* rising or falling */
        for (i = 0; i < m; i++)
            strength[i] = (int)rnd(0, n);
        for (i = 1; i < m; i++) /* insertion sort, rising */
            for (j = i; j > 0 && strength[j - 1] > strength[j]; j--) {
                v = strength[j];
                strength[j] = strength[j - 1];
                strength[j - 1] = v;
            }
        if (shape == 3)
            for (i = 0, j = m - 1; i < j; i++, j--) {
                v = strength[i];
                strength[i] = strength[j];
                strength[j] = v;
            }
    } else { /* all equal */
        s = (int)rnd(0, n);
        for (i = 0; i < m; i++)
            strength[i] = s;
    }
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            a[i * n + j] = j < strength[i];
    put_int_grid(a, m, n);
    put_int(rnd_chance(33) ? m : (int)rnd(1, m));
    free(strength);
    free(a);
}
