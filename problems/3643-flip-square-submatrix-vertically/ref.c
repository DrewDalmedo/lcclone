/* Reference solution: copy the grid, then walk two row pointers inward
 * from the block's top and bottom rows, swapping only the k cells of the
 * block in each pair of rows. */
int **reverseSubmatrix(int **grid, int gridSize, int *gridColSize, int x,
                       int y, int k, int *returnSize,
                       int **returnColumnSizes)
{
    int m = gridSize, n = gridColSize[0], i, top, bottom, c, t;
    int **out = malloc((size_t)m * sizeof *out);

    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        memcpy(out[i], grid[i], (size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
    }
    for (top = x, bottom = x + k - 1; top < bottom; top++, bottom--)
        for (c = y; c < y + k; c++) {
            t = out[top][c];
            out[top][c] = out[bottom][c];
            out[bottom][c] = t;
        }
    *returnSize = m;
    return out;
}
