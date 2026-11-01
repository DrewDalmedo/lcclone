/* Slow and obviously right: play the rounds. In each round, find every
 * row's largest number not yet given up and mark that copy as given up;
 * add the largest of those to the score. */
int deleteGreatestValue(int **grid, int gridSize, int *gridColSize)
{
    int gone[50][50] = {{0}};
    int n = gridColSize[0], score = 0, round, i, j, at, top;

    for (round = 0; round < n; round++) {
        top = 0;
        for (i = 0; i < gridSize; i++) {
            at = -1;
            for (j = 0; j < n; j++)
                if (!gone[i][j] && (at < 0 || grid[i][j] > grid[i][at]))
                    at = j;
            gone[i][at] = 1;
            if (grid[i][at] > top)
                top = grid[i][at];
        }
        score += top;
    }
    return score;
}
