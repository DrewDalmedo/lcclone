/* Reference solution: count the steps. Every blank should go the way the
 * forced steps already lean (either way on a tie), so the answer is
 * |#L - #R| + #blanks. */
int furthestDistanceFromOrigin(char *moves)
{
    int lead = 0, blanks = 0, i;

    for (i = 0; moves[i] != '\0'; i++) {
        if (moves[i] == 'L')
            lead--;
        else if (moves[i] == 'R')
            lead++;
        else
            blanks++;
    }
    return (lead < 0 ? -lead : lead) + blanks;
}
