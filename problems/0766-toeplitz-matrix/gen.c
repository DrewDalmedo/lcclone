/* A matrix of 1 to 5 by 1 to 5 cells with numbers 0 to 3 for small cases;
 * full-size ones are 20 by 20 most of the time, else of a random size up
 * to that, sometimes a single row or column, with numbers 0 to 99. Start
 * from a Toeplitz matrix, a random number for each slant, and then the
 * seed picks one of:
 *   - leave it so;
 *   - change one cell, anywhere, or in the last row or column, where a
 *     check that stops early or a loop bound that is one short misses it;
 *   - change a few cells, or every cell. */
void gen(int small)
{
    int m, n, top = small ? 3 : 99, mode = (int)rnd(0, 9), i, j, k, v, r, c;
    int *a, *slant;

    if (small) {
        m = rnd_chance(15) ? 1 : (int)rnd(2, 5);
        n = rnd_chance(15) ? 1 : (int)rnd(2, 5);
    } else if (rnd_chance(60)) {
        m = 20;
        n = 20;
    } else if (rnd_chance(25)) {
        m = rnd_chance(50) ? 1 : 20;
        n = m == 1 ? 20 : 1;
    } else {
        m = (int)rnd(1, 20);
        n = (int)rnd(1, 20);
    }
    a = malloc((size_t)m * n * sizeof *a);
    slant = malloc((size_t)(m + n - 1) * sizeof *slant);
    for (k = 0; k < m + n - 1; k++)
        slant[k] = (int)rnd(0, top);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            a[i * n + j] = slant[j - i + m - 1];

    if (mode >= 4 && mode <= 7) {
        /* one cell; on a full-size matrix, a number its slant doesn't
         * have yet */
        k = (int)rnd(0, 2);
        if (mode == 7 && k == 0) {
            r = m - 1;
            c = n - 1;
        } else if (mode == 7 && k == 1) {
            r = m - 1;
            c = (int)rnd(0, n - 1);
        } else if (mode == 7) {
            r = (int)rnd(0, m - 1);
            c = n - 1;
        } else {
            r = (int)rnd(0, m - 1);
            c = (int)rnd(0, n - 1);
        }
        v = (int)rnd(0, top);
        if (!small && v == a[r * n + c])
            v = (v + 1) % (top + 1);
        a[r * n + c] = v;
    } else if (mode == 8) {
        k = (int)rnd(1, 4);
        while (k-- > 0) {
            r = (int)rnd(0, m - 1);
            c = (int)rnd(0, n - 1);
            v = (int)rnd(0, top);
            a[r * n + c] = v;
        }
    } else if (mode == 9) {
        for (i = 0; i < m * n; i++)
            a[i] = (int)rnd(0, top);
    }
    put_int_grid(a, m, n);
    free(a);
    free(slant);
}
