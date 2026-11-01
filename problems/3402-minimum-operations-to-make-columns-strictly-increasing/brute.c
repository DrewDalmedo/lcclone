/* Slow and obviously right: copy the grid, then keep finding a cell that
 * is not above the cell over it and add 1 to it, one operation at a time,
 * until no such cell is left. Each step is forced: that cell must grow, and
 * it can only grow by operations on itself. */
int minimumOperations(int **grid, int gridSize, int *gridColSize)
{
    int m = gridSize, n = gridColSize[0], ops = 0, i, j, changed;
    int *g = malloc((size_t)m * (size_t)n * sizeof *g);

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            g[i * n + j] = grid[i][j];
    do {
        changed = 0;
        for (j = 0; j < n; j++)
            for (i = 1; i < m; i++)
                if (g[i * n + j] <= g[(i - 1) * n + j]) {
                    g[i * n + j]++;
                    ops++;
                    changed = 1;
                }
    } while (changed);
    free(g);
    return ops;
}
