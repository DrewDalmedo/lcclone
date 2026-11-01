/* Slow and obviously right: test both rules on every cell, straight from
 * the definition, skipping a rule where the neighbour doesn't exist. */
bool satisfiesConditions(int **grid, int gridSize, int *gridColSize)
{
    int m = gridSize, i, j;

    for (i = 0; i < m; i++)
        for (j = 0; j < gridColSize[i]; j++) {
            if (i + 1 < m && grid[i][j] != grid[i + 1][j])
                return false;
            if (j + 1 < gridColSize[i] && grid[i][j] == grid[i][j + 1])
                return false;
        }
    return true;
}
