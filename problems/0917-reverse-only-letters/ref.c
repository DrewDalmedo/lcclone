/* Reference solution: copy s, then walk one index up from the left and one
 * down from the right. An index on a non-letter moves on alone; when both
 * rest on letters, swap them and move both. */
static int is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

char *reverseOnlyLetters(char *s)
{
    int n = (int)strlen(s), lo = 0, hi = n - 1;
    char *r = malloc((size_t)n + 1), t;

    memcpy(r, s, (size_t)n + 1);
    while (lo < hi) {
        if (!is_letter(r[lo])) {
            lo++;
        } else if (!is_letter(r[hi])) {
            hi--;
        } else {
            t = r[lo];
            r[lo++] = r[hi];
            r[hi--] = t;
        }
    }
    return r;
}
