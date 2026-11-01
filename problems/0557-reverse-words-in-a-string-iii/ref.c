/* Reference solution: copy s, then walk along the copy; at the start of
 * each word find its end (the next space or the end of the string) and
 * reverse that stretch with two indices moving towards each other. */
char *reverseWords(char *s)
{
    size_t n = strlen(s), start = 0, end, i, j;
    char *out = malloc(n + 1), t;

    memcpy(out, s, n + 1);
    while (start < n) {
        end = start;
        while (end < n && out[end] != ' ')
            end++;
        i = start;
        j = end - 1;
        while (i < j) {
            t = out[i];
            out[i++] = out[j];
            out[j--] = t;
        }
        start = end + 1;
    }
    return out;
}
