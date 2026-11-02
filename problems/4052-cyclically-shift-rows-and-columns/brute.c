/* Slow and obviously right: copy grid, then slide each row left by one
 * place, rowShift[i] times over, and then each column up by one place,
 * colShift[j] times over. */
int **cyclicShift(int n, int **grid, int gridSize, int *gridColSize,
                  int *rowShift, int rowShiftSize, int *colShift,
                  int colShiftSize, int *returnSize, int **returnColumnSizes)
{
    int **a = malloc((size_t)n * sizeof *a);
    int i, j, t, first;

    (void)gridSize;
    (void)gridColSize;
    (void)rowShiftSize;
    (void)colShiftSize;
    *returnColumnSizes = malloc((size_t)n * sizeof **returnColumnSizes);
    for (i = 0; i < n; i++) {
        a[i] = malloc((size_t)n * sizeof **a);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++)
            a[i][j] = grid[i][j];
    }
    for (i = 0; i < n; i++)
        for (t = 0; t < rowShift[i]; t++) {
            first = a[i][0];
            for (j = 0; j + 1 < n; j++)
                a[i][j] = a[i][j + 1];
            a[i][n - 1] = first;
        }
    for (j = 0; j < n; j++)
        for (t = 0; t < colShift[j]; t++) {
            first = a[0][j];
            for (i = 0; i + 1 < n; i++)
                a[i][j] = a[i + 1][j];
            a[n - 1][j] = first;
        }
    *returnSize = n;
    return a;
}
