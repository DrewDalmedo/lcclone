/* Reference solution: from above, each cell with a tower shows one square.
 * Along the rows, row i shows a column of squares as tall as its tallest
 * tower, and along the columns likewise. One pass over the grid finds the
 * count and every row's and column's maximum. */
int projectionArea(int **grid, int gridSize, int *gridColSize)
{
    int n = gridSize, total = 0, i, j, row_max, col_max[50] = {0};

    (void)gridColSize;
    for (i = 0; i < n; i++) {
        row_max = 0;
        for (j = 0; j < n; j++) {
            if (grid[i][j] > 0)
                total++;
            if (grid[i][j] > row_max)
                row_max = grid[i][j];
            if (grid[i][j] > col_max[j])
                col_max[j] = grid[i][j];
        }
        total += row_max;
    }
    for (j = 0; j < n; j++)
        total += col_max[j];
    return total;
}
