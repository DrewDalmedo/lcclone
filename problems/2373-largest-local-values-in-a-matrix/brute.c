/* Slow and obviously right: for each output cell, look at every cell of
 * the grid and keep the largest among those inside its 3 by 3 patch. */
int **largestLocal(int **grid, int gridSize, int *gridColSize,
                   int *returnSize, int **returnColumnSizes)
{
    int n = gridSize, m = gridSize - 2, best, i, j, r, c;
    int **out = malloc((size_t)m * sizeof *out);

    (void)gridColSize;
    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)m * sizeof **out);
        (*returnColumnSizes)[i] = m;
        for (j = 0; j < m; j++) {
            best = 0;
            for (r = 0; r < n; r++)
                for (c = 0; c < n; c++)
                    if (r >= i && r <= i + 2 && c >= j && c <= j + 2 &&
                        grid[r][c] > best)
                        best = grid[r][c];
            out[i][j] = best;
        }
    }
    *returnSize = m;
    return out;
}
