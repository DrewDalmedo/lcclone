/* Slow and obviously right: for every word, try every other word and every
 * starting position in it, comparing letter by letter. */
char **stringMatching(char **words, int wordsSize, int *returnSize)
{
    char **out = malloc((size_t)wordsSize * sizeof *out);
    int i, j, found;
    size_t a, b, start, k;

    *returnSize = 0;
    for (i = 0; i < wordsSize; i++) {
        found = 0;
        a = strlen(words[i]);
        for (j = 0; j < wordsSize && !found; j++) {
            if (j == i)
                continue;
            b = strlen(words[j]);
            for (start = 0; start + a <= b && !found; start++) {
                for (k = 0; k < a && words[j][start + k] == words[i][k]; k++)
                    ;
                if (k == a)
                    found = 1;
            }
        }
        if (found) {
            out[*returnSize] = malloc(a + 1);
            memcpy(out[*returnSize], words[i], a + 1);
            (*returnSize)++;
        }
    }
    return out;
}
