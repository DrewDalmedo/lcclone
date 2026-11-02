/* Slow and obviously right: fill a new grid cell by cell, reading each
 * cell inside the block from its mirror row and every other cell from
 * the same place. */
int **reverseSubmatrix(int **grid, int gridSize, int *gridColSize, int x,
                       int y, int k, int *returnSize,
                       int **returnColumnSizes)
{
    int m = gridSize, n = gridColSize[0], i, j;
    int **out = malloc((size_t)m * sizeof *out);

    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++) {
            if (i >= x && i < x + k && j >= y && j < y + k)
                out[i][j] = grid[x + (x + k - 1 - i)][j];
            else
                out[i][j] = grid[i][j];
        }
    }
    *returnSize = m;
    return out;
}
