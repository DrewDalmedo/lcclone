/* Slow and obviously right: lay out the n wedge edges as directions
 * around the centre, measured in steps of 1/(2n) of a turn (edge i points
 * at step 2i, and the opposite direction is n steps on). Cover the edges
 * one at a time: an uncovered edge whose opposite direction is also an
 * uncovered edge gets a long cut for both, any other a short cut. A long
 * cut is never used where it would add an edge that isn't wanted. One
 * slice has no edges at all, so it needs no cut. */
int numberOfCuts(int n)
{
    int wanted[200] = {0}, covered[200] = {0};
    int cuts = 0, p, q;

    if (n == 1)
        return 0;
    for (p = 0; p < 2 * n; p += 2)
        wanted[p] = 1;
    for (p = 0; p < 2 * n; p++) {
        if (!wanted[p] || covered[p])
            continue;
        q = (p + n) % (2 * n);
        covered[p] = 1;
        if (wanted[q] && !covered[q])
            covered[q] = 1;
        cuts++;
    }
    return cuts;
}
