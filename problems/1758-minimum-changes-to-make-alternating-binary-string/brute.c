/* Slow and obviously right, for the short strings of the cross-check: try
 * every string of 0s and 1s of the same length, keep the alternating ones,
 * and count the positions where each differs from s. */
int minOperations(char *s)
{
    int n = (int)strlen(s), best = n, mask, i, ok, diff;

    for (mask = 0; mask < (1 << n); mask++) {
        ok = 1;
        for (i = 1; i < n; i++)
            if (((mask >> i) & 1) == ((mask >> (i - 1)) & 1))
                ok = 0;
        if (!ok)
            continue;
        diff = 0;
        for (i = 0; i < n; i++)
            if (s[i] - '0' != ((mask >> i) & 1))
                diff++;
        if (diff < best)
            best = diff;
    }
    return best;
}
