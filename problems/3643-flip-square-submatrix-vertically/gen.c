/* A random grid and block. Small cases have 1 to 5 rows and columns
 * (rarely just 1); full-size ones up to 50 of each (often exactly 50).
 * Values are 1..100, or (one time in five) 1..3, so that repeats come up.
 * The seed places the block:
 *   - anywhere, with any size that fits (usually at least 2 when the grid
 *     allows it);
 *   - from a random top-left tile, as large as fits, so that it touches
 *     the bottom or the right edge;
 *   - at the top-left corner, as large as fits (the whole grid when it is
 *     square);
 *   - at the bottom-right corner, with a random size. */
static int dim(int small)
{
    if (small)
        return rnd_chance(15) ? 1 : (int)rnd(2, 5);
    return rnd_chance(50) ? 50 : (int)rnd(1, 50);
}

static int side(int most)
{
    if (most >= 2 && rnd_chance(80))
        return (int)rnd(2, most);
    return (int)rnd(1, most);
}

void gen(int small)
{
    int grid[50 * 50], m, n, x, y, k, top, shape, i;

    m = dim(small);
    n = rnd_chance(30) ? m : dim(small);
    top = rnd_chance(20) ? 3 : 100;
    for (i = 0; i < m * n; i++)
        grid[i] = (int)rnd(1, top);
    shape = (int)rnd(0, 3);
    k = m < n ? m : n;
    if (shape == 0) {
        k = side(k);
        x = (int)rnd(0, m - k);
        y = (int)rnd(0, n - k);
    } else if (shape == 1) {
        x = (int)rnd(0, m - 1);
        y = (int)rnd(0, n - 1);
        k = m - x < n - y ? m - x : n - y;
    } else if (shape == 2) {
        x = y = 0;
    } else {
        k = side(k);
        x = m - k;
        y = n - k;
    }
    put_int_grid(grid, m, n);
    put_int(x);
    put_int(y);
    put_int(k);
}
