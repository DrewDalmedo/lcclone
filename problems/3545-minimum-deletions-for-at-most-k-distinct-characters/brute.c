/* Slow and obviously right: try every set of positions to keep, and among
 * those whose characters use at most k different letters, take the one
 * that keeps the most; everything else is deleted. */
int minDeletion(char *s, int k)
{
    int n = (int)strlen(s), best = n, mask, i, kept, distinct;
    int seen[26];

    for (mask = 0; mask < 1 << n; mask++) {
        memset(seen, 0, sizeof seen);
        kept = distinct = 0;
        for (i = 0; i < n; i++) {
            if (!(mask >> i & 1))
                continue;
            kept++;
            if (!seen[s[i] - 'a']++)
                distinct++;
        }
        if (distinct <= k && n - kept < best)
            best = n - kept;
    }
    return best;
}
