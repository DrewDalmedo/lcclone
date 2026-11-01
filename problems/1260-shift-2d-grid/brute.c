/* Slow and obviously right: make the k shifts one at a time. Each shift
 * fills a fresh grid in which every cell takes the value of the cell
 * before it: the cell to its left, or for the first cell of a row the last
 * cell of the row above, or for the top-left cell the bottom-right one. */
int **shiftGrid(int **grid, int gridSize, int *gridColSize, int k,
                int *returnSize, int **returnColumnSizes)
{
    int m = gridSize, n = gridColSize[0], step, i, j;
    int cur[50][50], next[50][50];
    int **out = malloc((size_t)m * sizeof *out);

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            cur[i][j] = grid[i][j];
    for (step = 0; step < k; step++) {
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++) {
                if (j > 0)
                    next[i][j] = cur[i][j - 1];
                else if (i > 0)
                    next[i][j] = cur[i - 1][n - 1];
                else
                    next[i][j] = cur[m - 1][n - 1];
            }
        memcpy(cur, next, sizeof cur);
    }
    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++)
            out[i][j] = cur[i][j];
    }
    *returnSize = m;
    return out;
}
