/* Slow and obviously right: write down the word index of every occurrence
 * of x, letter by letter, then drop the repeated indices. */
int *findWordsContaining(char **words, int wordsSize, char x, int *returnSize)
{
    size_t total = 1, j;
    int *hits, *out = malloc((size_t)wordsSize * sizeof *out);
    int nhits = 0, i, k, seen;

    for (i = 0; i < wordsSize; i++)
        total += strlen(words[i]);
    hits = malloc(total * sizeof *hits);
    for (i = 0; i < wordsSize; i++)
        for (j = 0; words[i][j] != '\0'; j++)
            if (words[i][j] == x)
                hits[nhits++] = i;
    *returnSize = 0;
    for (i = 0; i < nhits; i++) {
        seen = 0;
        for (k = 0; k < *returnSize; k++)
            if (out[k] == hits[i])
                seen = 1;
        if (!seen)
            out[(*returnSize)++] = hits[i];
    }
    free(hits);
    return out;
}
