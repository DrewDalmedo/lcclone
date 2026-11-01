/* Reference solution: walk each column from the top. The top cell stays
 * as it is; every lower cell must rise to at least one more than the
 * (possibly raised) cell above it, and raising it any higher only costs
 * more, so take the smallest value that works and count the difference. */
int minimumOperations(int **grid, int gridSize, int *gridColSize)
{
    int m = gridSize, n = gridColSize[0], total = 0, i, j, cur;

    for (j = 0; j < n; j++) {
        cur = grid[0][j];
        for (i = 1; i < m; i++) {
            if (grid[i][j] > cur)
                cur = grid[i][j];
            else {
                cur++;
                total += cur - grid[i][j];
            }
        }
    }
    return total;
}
