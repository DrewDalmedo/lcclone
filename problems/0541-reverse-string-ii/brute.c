/* Slow and obviously right: go through the groups of k characters one by
 * one, appending each odd-numbered group from its last character to its
 * first and each even-numbered one as it is. */
char *reverseStr(char *s, int k)
{
    long n = (long)strlen(s), g, start, end, i, m = 0;
    char *out = malloc((size_t)n + 1);

    for (g = 0; g * k < n; g++) {
        start = g * k;
        end = start + k < n ? start + k : n;
        if (g % 2 == 0)
            for (i = end - 1; i >= start; i--)
                out[m++] = s[i];
        else
            for (i = start; i < end; i++)
                out[m++] = s[i];
    }
    out[m] = '\0';
    return out;
}
