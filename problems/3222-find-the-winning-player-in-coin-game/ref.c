/* Reference solution: 115 is not a multiple of 10 and two 75s are too
 * much, so every turn takes one 75 and four 10s. The game lasts
 * min(x, y / 4) turns, and Alice wins when that count is odd. */
char *winningPlayer(int x, int y)
{
    int turns = x < y / 4 ? x : y / 4;

    return turns % 2 == 1 ? "Alice" : "Bob";
}
