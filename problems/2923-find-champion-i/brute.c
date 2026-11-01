/* Slow and obviously right: straight from the definition, return the player
 * whose column has no 1, i.e. that no other player is rated above. */
int findChampion(int **grid, int gridSize, int *gridColSize)
{
    int c, i, above;

    (void)gridColSize;
    for (c = 0; c < gridSize; c++) {
        above = 0;
        for (i = 0; i < gridSize; i++)
            if (i != c && grid[i][c] == 1)
                above = 1;
        if (!above)
            return c;
    }
    return -1;
}
