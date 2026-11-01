/* A square grid of heights 1..100. Small cases: n from 3 to 6, with the
 * heights, by the seed, all different (so a transposed or shifted answer
 * shows), drawn from a few small values (ties), or mostly 1 with a few
 * spikes up to 100. Full-size cases: n = 100, with random heights, low
 * heights and a single 100 in one corner of the grid, or one height
 * everywhere. */
void gen(int small)
{
    static int grid[100 * 100];
    int n, shape, top, corner, i;

    n = small ? (int)rnd(3, 6) : 100;
    shape = (int)rnd(0, 2);
    top = small ? (int)rnd(2, 9) : (int)rnd(1, 100);
    for (i = 0; i < n * n; i++) {
        if (shape == 0)
            grid[i] = small ? i + 1 : (int)rnd(1, 100);
        else if (shape == 1)
            grid[i] = small ? (int)rnd(1, top) : (int)rnd(1, 50);
        else if (small)
            grid[i] = rnd_chance(15) ? (int)rnd(2, 100) : 1;
        else
            grid[i] = top;
    }
    if (small && shape == 0)
        rnd_shuffle(grid, n * n);
    if (!small && shape == 1) {
        corner = (int)rnd(0, 3);
        grid[(corner / 2) * (n - 1) * n + (corner % 2) * (n - 1)] = 100;
    }
    put_int_grid(grid, n, n);
}
