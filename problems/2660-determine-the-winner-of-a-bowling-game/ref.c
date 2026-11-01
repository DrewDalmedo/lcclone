/* Reference solution: score each player in one pass, keeping how many of
 * the coming rounds are still doubled. A strike sets that to 2 (it never
 * adds up, so a round is doubled at most once); every round uses up one. */
static int score(const int *pins, int n)
{
    int total = 0, doubled = 0, i;

    for (i = 0; i < n; i++) {
        total += doubled > 0 ? 2 * pins[i] : pins[i];
        if (doubled > 0)
            doubled--;
        if (pins[i] == 10)
            doubled = 2;
    }
    return total;
}

int isWinner(int *player1, int player1Size, int *player2, int player2Size)
{
    int a = score(player1, player1Size), b = score(player2, player2Size);

    if (a > b)
        return 1;
    if (b > a)
        return 2;
    return 0;
}
