/* Slow and obviously right: try every non-empty set of digits to keep,
 * and among the kept strings that end in 2 take the largest number
 * (longer wins; at equal length, the larger digit at the first place they
 * differ wins). Only for the short strings of the cross-check. */
char *largestEven(char *s)
{
    int n = (int)strlen(s), blen = 0, len, i;
    long mask;
    char *best = malloc((size_t)n + 1), *cur = malloc((size_t)n + 1);

    best[0] = '\0';
    for (mask = 1; mask < (1L << n); mask++) {
        len = 0;
        for (i = 0; i < n; i++)
            if (mask & (1L << i))
                cur[len++] = s[i];
        cur[len] = '\0';
        if (cur[len - 1] != '2')
            continue;
        if (len > blen || (len == blen && strcmp(cur, best) > 0)) {
            strcpy(best, cur);
            blen = len;
        }
    }
    free(cur);
    return best;
}
