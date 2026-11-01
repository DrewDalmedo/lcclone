/* A random grid. Small cases are at most 5 x 5 with values that tie
 * often (0..5), and now and then a top row near 2499 so the raised values
 * pass 2500. Full-size cases are 50 x 50, shaped by the seed: random
 * values, a top row of 2499 over small values (the largest answers),
 * columns that fall from top to bottom, or one value everywhere. */
void gen(int small)
{
    int m, n, i, j, shape, v, *g;

    if (small) {
        m = (int)rnd(1, 5);
        n = (int)rnd(1, 5);
    } else {
        m = 50;
        n = 50;
    }
    shape = (int)rnd(0, 3);
    v = (int)rnd(0, 2499);
    g = malloc((size_t)m * (size_t)n * sizeof *g);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++) {
            int x;

            if (small) {
                x = (int)rnd(0, 5);
                if (i == 0 && shape == 0)
                    x = 2499 - x;
            } else if (shape == 0) {
                x = (int)rnd(0, 2499);
            } else if (shape == 1) {
                x = i == 0 ? 2499 : (int)rnd(0, 20);
            } else if (shape == 2) {
                x = 2499 - 50 * i - (int)rnd(0, 49);
            } else {
                x = v;
            }
            g[i * n + j] = x;
        }
    put_int_grid(g, m, n);
    free(g);
}
