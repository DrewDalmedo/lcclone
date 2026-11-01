/* Slow and obviously right: a full game search. A position (a, b) of a
 * coins worth 75 and b worth 10 is a win for the player to move when some
 * handful of i 75s and j 10s worth exactly 115 leaves a position that is a
 * loss for the other player; results are memoized. */
static signed char memo[101][101];

static int wins(int a, int b)
{
    int i, j, result = 0;

    if (memo[a][b])
        return memo[a][b] > 0;
    for (i = 0; i <= a && !result; i++)
        for (j = 0; j <= b && !result; j++)
            if (75 * i + 10 * j == 115 && !wins(a - i, b - j))
                result = 1;
    memo[a][b] = result ? 1 : -1;
    return result;
}

char *winningPlayer(int x, int y)
{
    return wins(x, y) ? "Alice" : "Bob";
}
