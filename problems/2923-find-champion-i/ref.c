/* Reference solution: keep a candidate and walk through the players; when
 * player i is rated above the candidate, i takes its place. Because the
 * verdicts form one order, the candidate at the end has nobody above it. */
int findChampion(int **grid, int gridSize, int *gridColSize)
{
    int champion = 0, i;

    (void)gridColSize;
    for (i = 1; i < gridSize; i++)
        if (grid[i][champion] == 1)
            champion = i;
    return champion;
}
