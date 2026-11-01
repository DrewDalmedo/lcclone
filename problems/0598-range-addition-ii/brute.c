/* Slow and obviously right: build the grid, add 1 to every cell of each
 * rectangle, then find the largest number and count the cells holding it.
 * Only for small grids. */
int maxCount(int m, int n, int **ops, int opsSize, int *opsColSize)
{
    int *grid = calloc((size_t)m * n, sizeof *grid);
    int best, count = 0, i, x, y;

    (void)opsColSize;
    for (i = 0; i < opsSize; i++)
        for (x = 0; x < ops[i][0]; x++)
            for (y = 0; y < ops[i][1]; y++)
                grid[x * n + y]++;
    best = grid[0];
    for (i = 0; i < m * n; i++)
        if (grid[i] > best)
            best = grid[i];
    for (i = 0; i < m * n; i++)
        if (grid[i] == best)
            count++;
    free(grid);
    return count;
}
