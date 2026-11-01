/* Reference solution: count each letter, then sweep the 26 counts from a
 * up to z and from z back down to a, again and again, taking one copy of
 * every letter that still has one on each visit, until all n letters
 * have been written. */
char *sortString(char *s)
{
    int cnt[26] = {0};
    size_t n = strlen(s), k = 0, i;
    char *out = malloc(n + 1);
    int c;

    for (i = 0; i < n; i++)
        cnt[s[i] - 'a']++;
    while (k < n) {
        for (c = 0; c < 26; c++)
            if (cnt[c] > 0) {
                cnt[c]--;
                out[k++] = (char)('a' + c);
            }
        for (c = 25; c >= 0; c--)
            if (cnt[c] > 0) {
                cnt[c]--;
                out[k++] = (char)('a' + c);
            }
    }
    out[k] = '\0';
    return out;
}
