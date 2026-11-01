/* Reference solution: walk along s keeping the number of Rs minus the
 * number of Ls seen so far. A piece can end only where that difference is
 * back to 0, and cutting at every such place keeps every piece balanced,
 * so the answer is the number of places where it is 0. */
int balancedStringSplit(char *s)
{
    int diff = 0, pieces = 0, i;

    for (i = 0; s[i] != '\0'; i++) {
        diff += s[i] == 'R' ? 1 : -1;
        if (diff == 0)
            pieces++;
    }
    return pieces;
}
