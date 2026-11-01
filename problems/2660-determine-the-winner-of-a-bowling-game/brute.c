/* Slow and obviously right: straight from the rule. For each round, look
 * back at the two rounds before it (those that exist) and double the round
 * if either of them has 10 pins. */
static int score(const int *pins, int n)
{
    int total = 0, i, back, doubled;

    for (i = 0; i < n; i++) {
        doubled = 0;
        for (back = 1; back <= 2; back++)
            if (i - back >= 0 && pins[i - back] == 10)
                doubled = 1;
        total += doubled ? 2 * pins[i] : pins[i];
    }
    return total;
}

int isWinner(int *player1, int player1Size, int *player2, int player2Size)
{
    int a = score(player1, player1Size), b = score(player2, player2Size);

    return a > b ? 1 : a < b ? 2 : 0;
}
