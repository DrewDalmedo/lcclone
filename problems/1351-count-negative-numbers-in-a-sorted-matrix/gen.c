/* A grid that never increases to the right or downward: each cell is the
 * smaller of its upper and left neighbours, minus a random drop (often 0,
 * so equal values are common), and never below the lowest allowed value.
 * Small cases have up to 5 x 5 cells with values from -4 to 5, so zeros,
 * grids without negatives and grids of negatives alone come up often.
 * Full-size ones are 100 x 100, a single row or column of 100, or a
 * random shape; they start positive, with drops sized so that the sign
 * changes around the middle of the grid. */
void gen(int small)
{
    int m, n, top, bottom, half = 1, i, j, v, d, shape;
    int *g;

    if (small) {
        top = (int)rnd(-2, 5);
        bottom = -4;
        m = (int)rnd(1, 5);
        n = (int)rnd(1, 5);
    } else {
        top = (int)rnd(1, 100);
        bottom = -100;
        shape = (int)rnd(0, 3);
        m = shape == 1 ? 1 : 100;
        n = shape == 2 ? 1 : 100;
        if (shape == 3) {
            m = (int)rnd(1, 100);
            n = (int)rnd(1, 100);
        }
        /* A row or column drops by its one neighbour; a cell with two
         * takes the lower, which drops about twice as fast. */
        half = m == 1 || n == 1 ? (m + n - 2) / 2 : m + n - 2;
        if (half < 1)
            half = 1;
    }
    g = malloc((size_t)m * (size_t)n * sizeof *g);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++) {
            if (i == 0 && j == 0) {
                g[0] = top;
                continue;
            }
            if (i == 0)
                v = g[j - 1];
            else if (j == 0)
                v = g[(i - 1) * n];
            else
                v = g[(i - 1) * n + j] < g[i * n + j - 1]
                        ? g[(i - 1) * n + j] : g[i * n + j - 1];
            if (small)
                d = rnd_chance(50) ? 0 : (int)rnd(1, 2);
            else if (top >= half)
                d = (int)rnd(0, 2 * top / half);
            else
                d = rnd_chance(100 * top / half) ? 1 : 0;
            v -= d;
            g[i * n + j] = v < bottom ? bottom : v;
        }
    put_int_grid(g, m, n);
    free(g);
}
