/* n is 1 to 5 in small cases (3 to 5 most of the time, as left and right
 * slides look the same when n is 1 or 2), and 10 in full-size ones (or now
 * and then any n from 1 to 10). Most grids hold different numbers, drawn
 * from 1 to 100 in a random order, so that a tile in the wrong place shows;
 * else numbers from 1 to 3, which repeat. Each shift is random from 0 to
 * n - 1, or n - 1 one time in five; one time in six all the row shifts are
 * 0, and one time in six all the column shifts. */
void gen(int small)
{
    int n, i, k, v, top, zeros;
    int pool[100], grid[100], rowShift[10], colShift[10];

    if (small)
        n = rnd_chance(20) ? (int)rnd(1, 2) : (int)rnd(3, 5);
    else
        n = rnd_chance(80) ? 10 : (int)rnd(1, 10);
    for (i = 0; i < 100; i++)
        pool[i] = i + 1;
    rnd_shuffle(pool, 100);
    top = rnd_chance(80) ? 0 : 3;
    for (i = 0; i < n * n; i++)
        grid[i] = top ? (int)rnd(1, top) : pool[i];
    v = (int)rnd(0, 5);
    zeros = v == 0 ? 1 : v == 1 ? 2 : 0;
    for (i = 0; i < n; i++) {
        k = rnd_chance(20) ? n - 1 : (int)rnd(0, n - 1);
        rowShift[i] = zeros == 1 ? 0 : k;
    }
    for (i = 0; i < n; i++) {
        k = rnd_chance(20) ? n - 1 : (int)rnd(0, n - 1);
        colShift[i] = zeros == 2 ? 0 : k;
    }
    put_int(n);
    put_int_grid(grid, n, n);
    put_ints(rowShift, n);
    put_ints(colShift, n);
}
