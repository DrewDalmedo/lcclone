/* A grid of 1 to 4 by 1 to 4 cells for small cases, with values from -3
 * to 3 half the time (so values repeat), else from -1000 to 1000.
 * Full-size grids are 50 by 50 most of the time, else 1 by 50, 50 by 1 or
 * of a random size, with values from -1000 to 1000, or from -9 to 9 one
 * time in three (short enough for tests.txt to hold a 50 by 50 case). The
 * seed picks k:
 *   0-4: any k from 0 to 100;
 *   5:   0;
 *   6:   a multiple of m * n (when one is at most 100), so nothing moves;
 *   7:   a multiple of n, so whole rows move down;
 *   8:   one less than a multiple of m * n, a shift back by one cell;
 *   9:   100. */
void gen(int small)
{
    int m, n, k, total, top, i, shape;
    int *grid;

    if (small) {
        m = (int)rnd(1, 4);
        n = (int)rnd(1, 4);
    } else if (rnd_chance(60)) {
        m = 50;
        n = 50;
    } else if (rnd_chance(50)) {
        m = (int)rnd(1, 50);
        n = (int)rnd(1, 50);
    } else if (rnd_chance(50)) {
        m = 1;
        n = 50;
    } else {
        m = 50;
        n = 1;
    }
    total = m * n;
    if (small)
        top = rnd_chance(50) ? 3 : 1000;
    else
        top = rnd_chance(33) ? 9 : 1000;
    grid = malloc((size_t)total * sizeof *grid);
    for (i = 0; i < total; i++)
        grid[i] = (int)rnd(-top, top);
    shape = (int)rnd(0, 9);
    if (shape == 5)
        k = 0;
    else if (shape == 6 && total <= 100)
        k = total * (int)rnd(0, 100 / total);
    else if (shape == 7 && n <= 100)
        k = n * (int)rnd(0, 100 / n);
    else if (shape == 8 && total <= 101)
        k = total * (int)rnd(1, 101 / total) - 1;
    else if (shape == 9)
        k = 100;
    else
        k = (int)rnd(0, 100);
    put_int_grid(grid, m, n);
    put_int(k);
    free(grid);
}
