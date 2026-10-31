/* Slow and obviously right: go through the letters of the first word one
 * at a time, and keep a letter when every other word still has a copy of
 * it that hasn't been used; then cross those copies out. */
char **commonChars(char **words, int wordsSize, int *returnSize)
{
    size_t len0 = strlen(words[0]), i;
    char **left = malloc((size_t)wordsSize * sizeof *left);
    char **out = malloc(len0 * sizeof *out);
    int n = 0, ok, w;
    char c;

    for (w = 0; w < wordsSize; w++) {
        left[w] = malloc(strlen(words[w]) + 1);
        strcpy(left[w], words[w]);
    }
    for (i = 0; i < len0; i++) {
        c = words[0][i];
        ok = 1;
        for (w = 1; w < wordsSize; w++)
            if (!strchr(left[w], c))
                ok = 0;
        if (!ok)
            continue;
        for (w = 1; w < wordsSize; w++)
            *strchr(left[w], c) = '#';
        out[n] = malloc(2);
        out[n][0] = c;
        out[n][1] = '\0';
        n++;
    }
    for (w = 0; w < wordsSize; w++)
        free(left[w]);
    free(left);
    *returnSize = n;
    return out;
}
