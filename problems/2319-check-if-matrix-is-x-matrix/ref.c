/* Reference solution: one pass over the cells; a marked cell must be
 * non-zero and any other cell zero. */
bool checkXMatrix(int **grid, int gridSize, int *gridColSize)
{
    int n = gridSize, i, j, marked;

    (void)gridColSize;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            marked = i == j || i + j == n - 1;
            if (marked != (grid[i][j] != 0))
                return false;
        }
    return true;
}
