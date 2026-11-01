/* Reference solution: find the first ch, copy word, then swap the two ends
 * of the head inward until they meet. */
char *reversePrefix(char *word, char ch)
{
    size_t n = strlen(word), p = 0, lo, hi;
    char *r = malloc(n + 1), t;

    memcpy(r, word, n + 1);
    while (p < n && word[p] != ch)
        p++;
    if (p == n)
        return r;
    for (lo = 0, hi = p; lo < hi; lo++, hi--) {
        t = r[lo];
        r[lo] = r[hi];
        r[hi] = t;
    }
    return r;
}
