/* Reference solution: copy s, then walk two indices in from both ends of
 * the first k characters, swapping as they go, until they meet. */
char *reversePrefix(char *s, int k)
{
    size_t n = strlen(s);
    char *out = malloc(n + 1);
    char t;
    int lo = 0, hi = k - 1;

    memcpy(out, s, n + 1);
    while (lo < hi) {
        t = out[lo];
        out[lo] = out[hi];
        out[hi] = t;
        lo++;
        hi--;
    }
    return out;
}
