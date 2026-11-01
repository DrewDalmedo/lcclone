/* Small grids are 1 to 4 rows by 1 to 4 columns, with amounts up to 6
 * (so that wealths tie often) or, a quarter of the time, up to 100 (so
 * that a wealth can pass 255). Full-size grids come in a shape the seed
 * picks: 50 by 50 with random amounts; wide or tall, with one side 50 and
 * the other random; the same amount everywhere, except that one customer
 * (often the last) may have a few amounts one higher; or one bank that
 * holds 100 for every customer among small amounts elsewhere, so that the
 * largest column total is far above every row total. */
void gen(int small)
{
    int m, n, shape, top, i, j, v, rich, col, *g;

    if (small) {
        m = (int)rnd(1, 4);
        n = (int)rnd(1, 4);
        top = rnd_chance(25) ? 100 : 6;
        g = malloc((size_t)m * n * sizeof *g);
        for (i = 0; i < m * n; i++)
            g[i] = (int)rnd(1, top);
    } else {
        shape = (int)(gen_seed() % 4);
        m = 50;
        n = 50;
        if (shape == 1) {
            if (rnd_chance(50))
                m = (int)rnd(1, 50);
            else
                n = (int)rnd(1, 50);
        }
        g = malloc((size_t)m * n * sizeof *g);
        v = (int)rnd(1, 100);
        rich = rnd_chance(50) ? m - 1 : (int)rnd(0, m - 1);
        col = (int)rnd(0, n - 1);
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++) {
                if (shape <= 1)
                    g[i * n + j] = (int)rnd(1, 100);
                else if (shape == 2)
                    g[i * n + j] = v;
                else if (j == col)
                    g[i * n + j] = 100;
                else
                    g[i * n + j] = (int)rnd(1, 3);
            }
        if (shape == 2 && v < 100) {
            /* One customer a little richer than the rest, by moving a few
             * of their amounts up. */
            for (j = 0; j < n; j++)
                if (rnd_chance(10))
                    g[rich * n + j] = v + 1;
        }
    }
    put_int_grid(g, m, n);
    free(g);
}
