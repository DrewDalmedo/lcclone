/* Slow and obviously right: try every start i holding 1 and every stop j
 * holding -1, check that every position between them holds 0, and count
 * those positions. */
int captureForts(int *forts, int fortsSize)
{
    int best = 0, i, j, k, lo, hi, clear;

    for (i = 0; i < fortsSize; i++)
        for (j = 0; j < fortsSize; j++) {
            if (forts[i] != 1 || forts[j] != -1)
                continue;
            lo = i < j ? i : j;
            hi = i < j ? j : i;
            clear = 1;
            for (k = lo + 1; k < hi; k++)
                if (forts[k] != 0)
                    clear = 0;
            if (clear && hi - lo - 1 > best)
                best = hi - lo - 1;
        }
    return best;
}
