/* Slow and obviously right: rewrite every word in the usual alphabet (the
 * letter at position p of order becomes 'a' + p), then check with strcmp
 * that each word is <= every word after it, not only the next one. */
bool isAlienSorted(char **words, int wordsSize, char *order)
{
    char (*plain)[21] = malloc((size_t)wordsSize * sizeof *plain);
    const char *at;
    bool ok = true;
    int i, j, k;

    for (i = 0; i < wordsSize; i++) {
        for (k = 0; words[i][k]; k++) {
            at = strchr(order, words[i][k]);
            plain[i][k] = (char)('a' + (at - order));
        }
        plain[i][k] = '\0';
    }
    for (i = 0; i < wordsSize; i++)
        for (j = i + 1; j < wordsSize; j++)
            if (strcmp(plain[i], plain[j]) > 0)
                ok = false;
    free(plain);
    return ok;
}
