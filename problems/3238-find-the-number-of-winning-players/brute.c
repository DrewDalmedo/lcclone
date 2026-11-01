/* Slow and obviously right: for every player and every colour, go through
 * the whole list and count the matching draws. */
int winningPlayerCount(int n, int **pick, int pickSize, int *pickColSize)
{
    int winners = 0, player, colour, i, balls, wins;

    (void)pickColSize;
    for (player = 0; player < n; player++) {
        wins = 0;
        for (colour = 0; colour <= 10; colour++) {
            balls = 0;
            for (i = 0; i < pickSize; i++)
                if (pick[i][0] == player && pick[i][1] == colour)
                    balls++;
            if (balls >= player + 1)
                wins = 1;
        }
        winners += wins;
    }
    return winners;
}
