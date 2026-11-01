/* Reference solution: number the cells 0 to m * n - 1 row by row. A
 * shift moves the value of cell p to cell p + 1, wrapping from the last
 * cell to cell 0, so after k shifts the value of cell p sits in cell
 * (p + k) mod (m * n). Copy every value straight to that cell of a new
 * grid. */
int **shiftGrid(int **grid, int gridSize, int *gridColSize, int k,
                int *returnSize, int **returnColumnSizes)
{
    int m = gridSize, n = gridColSize[0], total = m * n, p, q, i;
    int **out = malloc((size_t)m * sizeof *out);

    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
    }
    k %= total;
    for (p = 0; p < total; p++) {
        q = (p + k) % total;
        out[q / n][q % n] = grid[p / n][p % n];
    }
    *returnSize = m;
    return out;
}
