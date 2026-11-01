/* Reference solution: a table gives the row (1, 2 or 3) of each letter,
 * looked up by the letter's lowercase form. A word is kept when every
 * letter has the row of its first letter. The kept entries point at the
 * input words themselves. */
char **findWords(char **words, int wordsSize, int *returnSize)
{
    /* the row of a, b, c, ..., z */
    static const char row[] = "23321222122233111121131313";
    char **out = malloc((size_t)wordsSize * sizeof *out);
    const char *w;
    char first;
    int i, n = 0;

    for (i = 0; i < wordsSize; i++) {
        w = words[i];
        first = row[tolower((unsigned char)w[0]) - 'a'];
        while (*w && row[tolower((unsigned char)*w) - 'a'] == first)
            w++;
        if (*w == '\0')
            out[n++] = words[i];
    }
    *returnSize = n;
    return out;
}
