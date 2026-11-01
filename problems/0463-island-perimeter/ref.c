/* Reference solution: each land cell has 4 sides, and each pair of land
 * cells that share a side hides 2 of them, one of each cell. Count the land
 * cells and those pairs (looking only right and down, so each pair once):
 * the coast is 4 * land - 2 * pairs. */
int islandPerimeter(int **grid, int gridSize, int *gridColSize)
{
    int rows = gridSize, cols = gridColSize[0], land = 0, pairs = 0, i, j;

    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++) {
            if (grid[i][j] == 0)
                continue;
            land++;
            if (j + 1 < cols && grid[i][j + 1] == 1)
                pairs++;
            if (i + 1 < rows && grid[i + 1][j] == 1)
                pairs++;
        }
    return 4 * land - 2 * pairs;
}
