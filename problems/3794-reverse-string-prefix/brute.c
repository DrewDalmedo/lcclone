/* Slow and obviously right: reverse the whole string; its last k
 * characters are then the first k of s backwards. Put those in front of
 * the part of s after its first k characters. */
char *reversePrefix(char *s, int k)
{
    int n = (int)strlen(s), i;
    char *all = malloc((size_t)n + 1);
    char *out = malloc((size_t)n + 1);

    for (i = 0; i < n; i++)
        all[i] = s[n - 1 - i];
    for (i = 0; i < k; i++)
        out[i] = all[n - k + i];
    for (i = k; i < n; i++)
        out[i] = s[i];
    out[n] = '\0';
    free(all);
    return out;
}
