/* Reference solution: no tile is moved one place at a time. After the
 * rows, row r and column c hold grid[r][(c + rowShift[r]) % n]; after the
 * columns, row i and column j hold what row (i + colShift[j]) % n of
 * column j held then. So each entry of the answer is read straight from
 * grid. */
int **cyclicShift(int n, int **grid, int gridSize, int *gridColSize,
                  int *rowShift, int rowShiftSize, int *colShift,
                  int colShiftSize, int *returnSize, int **returnColumnSizes)
{
    int **out = malloc((size_t)n * sizeof *out);
    int i, j, r;

    (void)gridSize;
    (void)gridColSize;
    (void)rowShiftSize;
    (void)colShiftSize;
    *returnColumnSizes = malloc((size_t)n * sizeof **returnColumnSizes);
    for (i = 0; i < n; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++) {
            r = (i + colShift[j]) % n;
            out[i][j] = grid[r][(j + rowShift[r]) % n];
        }
    }
    *returnSize = n;
    return out;
}
