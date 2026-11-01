/* Reference solution: count the balls of each (player, colour) pair in a
 * table of n rows and 11 columns, then count the players with some colour
 * whose count is above their own number. */
int winningPlayerCount(int n, int **pick, int pickSize, int *pickColSize)
{
    int *count = calloc((size_t)n * 11, sizeof *count);
    int winners = 0, i, c;

    (void)pickColSize;
    for (i = 0; i < pickSize; i++)
        count[pick[i][0] * 11 + pick[i][1]]++;
    for (i = 0; i < n; i++)
        for (c = 0; c <= 10; c++)
            if (count[i * 11 + c] > i) {
                winners++;
                break;
            }
    free(count);
    return winners;
}
