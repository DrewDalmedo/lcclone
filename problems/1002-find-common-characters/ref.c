/* Reference solution: count the letters of each word, keep the smallest
 * count of each letter over all the words, and write each letter out that
 * many times. */
char **commonChars(char **words, int wordsSize, int *returnSize)
{
    int least[26], count[26], total = 0, n = 0, c, w, k;
    const char *p;
    char **out;

    for (c = 0; c < 26; c++)
        least[c] = INT_MAX;
    for (w = 0; w < wordsSize; w++) {
        memset(count, 0, sizeof count);
        for (p = words[w]; *p; p++)
            count[*p - 'a']++;
        for (c = 0; c < 26; c++)
            if (count[c] < least[c])
                least[c] = count[c];
    }
    for (c = 0; c < 26; c++)
        total += least[c];
    out = malloc((size_t)(total + 1) * sizeof *out);
    for (c = 0; c < 26; c++)
        for (k = 0; k < least[c]; k++) {
            out[n] = malloc(2);
            out[n][0] = (char)('a' + c);
            out[n][1] = '\0';
            n++;
        }
    *returnSize = n;
    return out;
}
