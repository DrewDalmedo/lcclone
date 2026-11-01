/* Slow and obviously right: try each person as the judge and check both
 * conditions straight from the list: no pair starts with them, and for
 * every other person some pair says that person trusts them. */
int findJudge(int n, int **trust, int trustSize, int *trustColSize)
{
    int j, i, t, ok, found;

    (void)trustColSize;
    for (j = 1; j <= n; j++) {
        ok = 1;
        for (t = 0; t < trustSize; t++)
            if (trust[t][0] == j)
                ok = 0;
        for (i = 1; i <= n && ok; i++) {
            if (i == j)
                continue;
            found = 0;
            for (t = 0; t < trustSize; t++)
                if (trust[t][0] == i && trust[t][1] == j)
                    found = 1;
            if (!found)
                ok = 0;
        }
        if (ok)
            return j;
    }
    return -1;
}
