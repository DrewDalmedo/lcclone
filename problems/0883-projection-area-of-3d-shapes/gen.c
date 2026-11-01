/* Small boards are 1 by 1 to 4 by 4 with towers up to 5 cubes, bare about
 * half the time. Full-size boards are 50 by 50 most of the time, else of
 * a random size, in a shape the seed picks: towers of any height with a
 * random share of bare cells; every tower 50 cubes tall (the largest
 * answer); a few tall towers on a mostly bare board; or one row (or one
 * column) of tall towers among short ones, so that the rows and the
 * columns look different. */
void gen(int small)
{
    int n, shape, bare, i, j, v, line, by_row, *g;

    if (small) {
        n = (int)rnd(1, 4);
        g = malloc((size_t)n * n * sizeof *g);
        bare = (int)rnd(20, 70);
        for (i = 0; i < n * n; i++) {
            v = (int)rnd(1, 5);
            g[i] = rnd_chance(bare) ? 0 : v;
        }
    } else {
        n = rnd_chance(75) ? 50 : (int)rnd(1, 50);
        g = malloc((size_t)n * n * sizeof *g);
        shape = (int)(gen_seed() % 4);
        bare = (int)rnd(0, 90);
        line = (int)rnd(0, n - 1);
        by_row = rnd_chance(50);
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                if (shape == 0) {
                    v = (int)rnd(1, 50);
                    g[i * n + j] = rnd_chance(bare) ? 0 : v;
                } else if (shape == 1) {
                    g[i * n + j] = 50;
                } else if (shape == 2) {
                    v = (int)rnd(30, 50);
                    g[i * n + j] = rnd_chance(3) ? v : 0;
                } else if ((by_row ? i : j) == line) {
                    g[i * n + j] = (int)rnd(40, 50);
                } else {
                    g[i * n + j] = (int)rnd(0, 3);
                }
            }
    }
    put_int_grid(g, n, n);
    free(g);
}
