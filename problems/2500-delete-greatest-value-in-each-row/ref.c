/* Reference solution: sort each row in ascending order (in place). The
 * numbers given up in round k are then the entries of column n - k, so
 * the score is the sum, over the columns, of each column's maximum. */
static int ascending(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int deleteGreatestValue(int **grid, int gridSize, int *gridColSize)
{
    int n = gridColSize[0], score = 0, i, j, top;

    for (i = 0; i < gridSize; i++)
        qsort(grid[i], (size_t)n, sizeof grid[i][0], ascending);
    for (j = 0; j < n; j++) {
        top = grid[0][j];
        for (i = 1; i < gridSize; i++)
            if (grid[i][j] > top)
                top = grid[i][j];
        score += top;
    }
    return score;
}
