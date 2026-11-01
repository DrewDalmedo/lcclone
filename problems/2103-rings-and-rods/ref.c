/* Reference solution: keep a 3-bit mask of the colours seen on each rod,
 * one bit per colour, then count the rods whose mask has all three bits. */
int countPoints(char *rings)
{
    int mask[10] = {0}, i, complete = 0;

    for (i = 0; rings[i] && rings[i + 1]; i += 2) {
        int bit = rings[i] == 'R' ? 1 : rings[i] == 'G' ? 2 : 4;

        mask[rings[i + 1] - '0'] |= bit;
    }
    for (i = 0; i < 10; i++)
        if (mask[i] == 7)
            complete++;
    return complete;
}
