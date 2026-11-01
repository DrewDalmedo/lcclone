/* Slow and obviously right: go through every cube, and mark the square it
 * covers in each of the three pictures; then count the marked squares. */
int projectionArea(int **grid, int gridSize, int *gridColSize)
{
    static char top[50][50], along_rows[50][50], along_cols[50][50];
    int n = gridSize, i, j, k, total = 0;

    (void)gridColSize;
    memset(top, 0, sizeof top);
    memset(along_rows, 0, sizeof along_rows);
    memset(along_cols, 0, sizeof along_cols);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < grid[i][j]; k++) {
                top[i][j] = 1;          /* seen from above at (row, column) */
                along_rows[i][k] = 1;   /* seen along the rows at (row, height) */
                along_cols[j][k] = 1;   /* seen along the columns at (column, height) */
            }
    for (i = 0; i < 50; i++)
        for (j = 0; j < 50; j++)
            total += top[i][j] + along_rows[i][j] + along_cols[i][j];
    return total;
}
