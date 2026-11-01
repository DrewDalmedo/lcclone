/* Slow and obviously right: draw the X into a blank n x n picture, one
 * stroke at a time from the top row down, then compare the picture with
 * where grid's non-zero cells are. */
bool checkXMatrix(int **grid, int gridSize, int *gridColSize)
{
    int n = gridSize, i, j, k;
    char *x = calloc((size_t)n * n, 1);
    bool ok = true;

    (void)gridColSize;
    for (k = 0; k < n; k++) {
        x[k * n + k] = 1;
        x[k * n + (n - 1 - k)] = 1;
    }
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if ((grid[i][j] != 0) != (x[i * n + j] == 1))
                ok = false;
    free(x);
    return ok;
}
