/* Slow and obviously right: list the positions that hold vowels, then
 * fill the j-th of them (from the left) with the character at the j-th
 * from the right. */
char *reverseVowels(char *s)
{
    size_t n = strlen(s), k = 0, i, j;
    size_t *pos = malloc((n + 1) * sizeof *pos);
    char *r = malloc(n + 1);

    for (i = 0; i < n; i++)
        if (strchr("aeiouAEIOU", s[i]))
            pos[k++] = i;
    memcpy(r, s, n + 1);
    for (j = 0; j < k; j++)
        r[pos[j]] = s[pos[k - 1 - j]];
    free(pos);
    return r;
}
