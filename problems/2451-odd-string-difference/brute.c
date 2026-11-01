/* Slow and obviously right: write down the steps of every word, and for
 * each word count the other words with exactly the same steps. The common
 * steps are shared by at least two words, so the odd word is the one that
 * matches no other word. */
char *oddString(char **words, int wordsSize)
{
    int steps[100][19];
    int n = (int)strlen(words[0]), i, j, k, matches, equal;
    char *out;

    for (i = 0; i < wordsSize; i++)
        for (k = 0; k + 1 < n; k++)
            steps[i][k] = words[i][k + 1] - words[i][k];
    for (i = 0; i < wordsSize; i++) {
        matches = 0;
        for (j = 0; j < wordsSize; j++) {
            if (j == i)
                continue;
            equal = 1;
            for (k = 0; k + 1 < n; k++)
                if (steps[i][k] != steps[j][k])
                    equal = 0;
            matches += equal;
        }
        if (matches == 0) {
            out = malloc((size_t)n + 1);
            strcpy(out, words[i]);
            return out;
        }
    }
    out = malloc(1);
    out[0] = '\0';
    return out;
}
