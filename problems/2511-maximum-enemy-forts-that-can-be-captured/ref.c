/* Reference solution: one pass, remembering the last position that is
 * not 0. When the next such position holds the opposite value (a 1 and
 * a -1, in either order), every position between them is an enemy fort,
 * and an army can cross them all. */
int captureForts(int *forts, int fortsSize)
{
    int best = 0, last = -1, i;

    for (i = 0; i < fortsSize; i++) {
        if (forts[i] == 0)
            continue;
        if (last >= 0 && forts[last] == -forts[i] && i - last - 1 > best)
            best = i - last - 1;
        last = i;
    }
    return best;
}
