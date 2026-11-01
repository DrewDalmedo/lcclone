/* Reference solution: for each top-left corner (i, j), take the largest of
 * the 9 cells in rows i..i+2 and columns j..j+2. */
int **largestLocal(int **grid, int gridSize, int *gridColSize,
                   int *returnSize, int **returnColumnSizes)
{
    int m = gridSize - 2, best, i, j, r, c;
    int **out = malloc((size_t)m * sizeof *out);

    (void)gridColSize;
    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)m * sizeof **out);
        (*returnColumnSizes)[i] = m;
        for (j = 0; j < m; j++) {
            best = 0;
            for (r = i; r < i + 3; r++)
                for (c = j; c < j + 3; c++)
                    if (grid[r][c] > best)
                        best = grid[r][c];
            out[i][j] = best;
        }
    }
    *returnSize = m;
    return out;
}
