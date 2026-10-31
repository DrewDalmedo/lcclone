/* Reference solution: look for x in each word with strchr, and keep the
 * index of every word where it is found. */
int *findWordsContaining(char **words, int wordsSize, char x, int *returnSize)
{
    int *out = malloc((size_t)wordsSize * sizeof *out), i;

    *returnSize = 0;
    for (i = 0; i < wordsSize; i++)
        if (strchr(words[i], x) != NULL)
            out[(*returnSize)++] = i;
    return out;
}
