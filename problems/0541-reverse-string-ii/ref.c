/* Reference solution: copy s, then for each start i = 0, 2k, 4k, ...
 * reverse the copy from i to i + k - 1, or to the end of the string if that
 * comes first, with two indices that move towards each other. */
char *reverseStr(char *s, int k)
{
    size_t n = strlen(s), start, i, j;
    char *out = malloc(n + 1), t;

    memcpy(out, s, n + 1);
    for (start = 0; start < n; start += 2 * (size_t)k) {
        i = start;
        j = start + (size_t)k < n ? start + (size_t)k - 1 : n - 1;
        while (i < j) {
            t = out[i];
            out[i++] = out[j];
            out[j--] = t;
        }
    }
    return out;
}
