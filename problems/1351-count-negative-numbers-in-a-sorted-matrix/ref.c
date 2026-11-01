/* Reference solution: walk a staircase from the bottom-left corner. When
 * the cell is negative, it and everything to its right in that row are
 * negative, so count them and move up a row; otherwise move right. Each
 * step leaves a row or a column behind, so it takes O(m + n) steps. */
int countNegatives(int **grid, int gridSize, int *gridColSize)
{
    int n = gridColSize[0], i = gridSize - 1, j = 0, count = 0;

    while (i >= 0 && j < n) {
        if (grid[i][j] < 0) {
            count += n - j;
            i--;
        } else {
            j++;
        }
    }
    return count;
}
