/* Reference solution: turn each word into a 26-bit mask of the letters
 * it uses, sort the masks, and add up c * (c - 1) / 2 over every run of c
 * equal masks. */
static int by_mask(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int similarPairs(char **words, int wordsSize)
{
    int *mask = malloc((size_t)wordsSize * sizeof *mask);
    int pairs = 0, i, k, run;
    const char *s;

    for (i = 0; i < wordsSize; i++) {
        mask[i] = 0;
        for (s = words[i]; *s; s++)
            mask[i] |= 1 << (*s - 'a');
    }
    qsort(mask, (size_t)wordsSize, sizeof *mask, by_mask);
    for (i = 0; i < wordsSize; i = k) {
        for (k = i; k < wordsSize && mask[k] == mask[i]; k++)
            ;
        run = k - i;
        pairs += run * (run - 1) / 2;
    }
    free(mask);
    return pairs;
}
