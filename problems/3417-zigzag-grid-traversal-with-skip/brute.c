/* Slow and obviously right: write out the whole snake walk, then keep
 * every other value of it, starting with the first. */
int *zigzagTraversal(int **grid, int gridSize, int *gridColSize,
                     int *returnSize)
{
    int rows = gridSize, cols = gridColSize[0], r, c, n = 0, i, k = 0;
    int *walk = malloc((size_t)rows * cols * sizeof *walk);
    int *out = malloc((size_t)rows * cols * sizeof *out);

    for (r = 0; r < rows; r++)
        for (c = 0; c < cols; c++)
            walk[n++] = grid[r][r % 2 == 0 ? c : cols - 1 - c];
    for (i = 0; i < n; i++)
        if (i % 2 == 0)
            out[k++] = walk[i];
    free(walk);
    *returnSize = k;
    return out;
}
