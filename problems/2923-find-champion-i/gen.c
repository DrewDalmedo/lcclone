/* A random order of the players from top to bottom, and the grid it
 * gives: grid[i][j] = 1 when i comes before j in the order. Small: 2 to 8
 * players. Full size: 100, and some of the time the champion is the last
 * player, n - 1. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 100;
    int order[100], rank[100], *grid;
    int i, j, t;

    for (i = 0; i < n; i++)
        order[i] = i;
    rnd_shuffle(order, n);
    if (!small && rnd_chance(30)) {
        for (i = 0; order[i] != n - 1; i++)
            ;
        t = order[0];
        order[0] = order[i];
        order[i] = t;
    }
    for (i = 0; i < n; i++)
        rank[order[i]] = i;
    grid = malloc((size_t)n * n * sizeof *grid);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            grid[i * n + j] = rank[i] < rank[j];
    put_int_grid(grid, n, n);
    free(grid);
}
