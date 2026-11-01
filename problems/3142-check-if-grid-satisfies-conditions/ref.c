/* Reference solution: every column must repeat its top value all the way
 * down, and no two neighbouring values in the top row may be equal. */
bool satisfiesConditions(int **grid, int gridSize, int *gridColSize)
{
    int m = gridSize, n = gridColSize[0], i, j;

    for (j = 0; j < n; j++) {
        if (j + 1 < n && grid[0][j] == grid[0][j + 1])
            return false;
        for (i = 1; i < m; i++)
            if (grid[i][j] != grid[0][j])
                return false;
    }
    return true;
}
