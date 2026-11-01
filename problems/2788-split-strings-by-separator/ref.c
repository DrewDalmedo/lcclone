/* Reference solution: one scan of all the strings counts the chunks (a
 * chunk starts at a non-separator whose left neighbour is the start of
 * the string or a separator); a second scan copies each chunk out. */
char **splitWordsBySeparator(char **words, int wordsSize, char separator,
                             int *returnSize)
{
    char **out;
    int total = 0, m = 0, w, i, start;

    for (w = 0; w < wordsSize; w++)
        for (i = 0; words[w][i]; i++)
            if (words[w][i] != separator &&
                (i == 0 || words[w][i - 1] == separator))
                total++;
    out = malloc((size_t)(total > 0 ? total : 1) * sizeof *out);
    for (w = 0; w < wordsSize; w++) {
        i = 0;
        while (words[w][i]) {
            if (words[w][i] == separator) {
                i++;
                continue;
            }
            start = i;
            while (words[w][i] && words[w][i] != separator)
                i++;
            out[m] = malloc((size_t)(i - start) + 1);
            memcpy(out[m], words[w] + start, (size_t)(i - start));
            out[m][i - start] = '\0';
            m++;
        }
    }
    *returnSize = m;
    return out;
}
