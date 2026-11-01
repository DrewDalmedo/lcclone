/* Kinds of boxes and a truck size. Small cases have a few kinds with small
 * counts and few distinct units values, so ties and identical entries come
 * up, and a truck from 1 box to a little more than the whole stock.
 * Full-size cases have 1000 kinds; the seed picks the shape: every entry
 * [1000,1000] with a 10^6-box truck (the largest answer, 10^9, and the
 * slowest case for loading one box at a time), entries in increasing order
 * of units with a truck that takes nearly everything, or random entries
 * with a random truck. */
void gen(int small)
{
    int m = small ? (int)rnd(1, 6) : 1000, shape = (int)rnd(0, 2), i;
    int *grid = malloc((size_t)m * 2 * sizeof *grid);
    long long stock = 0, truck;

    for (i = 0; i < m; i++) {
        if (small) {
            grid[2 * i] = (int)rnd(1, 5);
            grid[2 * i + 1] = (int)rnd(1, 6);
        } else if (shape == 0) {
            grid[2 * i] = 1000;
            grid[2 * i + 1] = 1000;
        } else if (shape == 1) {
            grid[2 * i] = (int)rnd(900, 1000);
            grid[2 * i + 1] = i + 1;
        } else {
            grid[2 * i] = (int)rnd(1, 1000);
            grid[2 * i + 1] = (int)rnd(1, 1000);
        }
        stock += grid[2 * i];
    }
    if (small)
        truck = rnd(1, stock + 3);
    else if (shape == 0)
        truck = 1000000;
    else if (shape == 1)
        truck = stock - rnd(0, 1000);
    else
        truck = rnd(1, 1000000);
    if (truck < 1)
        truck = 1;
    put_int_grid(grid, m, 2);
    put_int(truck);
    free(grid);
}
