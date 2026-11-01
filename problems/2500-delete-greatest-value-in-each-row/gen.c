/* A grid of random numbers. Small cases are at most 4 x 5 with numbers
 * up to 6 (so that rows hold repeats and rows tie), now and then up to
 * 100. Full-size ones are 50 x 50 with numbers from 1 to 100; the seed
 * picks plain random rows, or rows that are all shuffles of one set of
 * numbers (so the largest numbers sit in different columns). */
void gen(int small)
{
    static int grid[50 * 50];
    int m, n, top, shape, i, j;

    m = small ? (int)rnd(1, 4) : 50;
    n = small ? (int)rnd(1, 5) : 50;
    top = small && rnd_chance(80) ? 6 : 100;
    shape = (int)rnd(0, 2);
    for (j = 0; j < n; j++)
        grid[j] = (int)rnd(1, top);
    for (i = 1; i < m; i++) {
        if (shape == 0) {           /* a shuffle of row 0 */
            for (j = 0; j < n; j++)
                grid[i * n + j] = grid[j];
            rnd_shuffle(grid + i * n, n);
        } else {                    /* independent random numbers */
            for (j = 0; j < n; j++)
                grid[i * n + j] = (int)rnd(1, top);
        }
    }
    put_int_grid(grid, m, n);
}
