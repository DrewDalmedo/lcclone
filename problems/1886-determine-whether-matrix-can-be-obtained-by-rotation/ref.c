/* Reference solution: one pass over the cells of target, comparing each
 * with the cell of mat that lands there after 0, 1, 2 and 3 clockwise
 * quarter turns, and keeping a flag for each turn count that still fits. */
bool findRotation(int **mat, int matSize, int *matColSize, int **target,
                  int targetSize, int *targetColSize)
{
    int n = matSize, i, j, v;
    bool fit0 = true, fit1 = true, fit2 = true, fit3 = true;

    (void)matColSize;
    (void)targetSize;
    (void)targetColSize;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            v = target[i][j];
            if (mat[i][j] != v)
                fit0 = false;
            if (mat[n - 1 - j][i] != v)
                fit1 = false;
            if (mat[n - 1 - i][n - 1 - j] != v)
                fit2 = false;
            if (mat[j][n - 1 - i] != v)
                fit3 = false;
        }
    return fit0 || fit1 || fit2 || fit3;
}
