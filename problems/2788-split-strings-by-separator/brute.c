/* Slow and obviously right: cut a copy of each string at every separator
 * with strtok, which skips empty pieces, and keep each piece it returns.
 * Every chunk has at least one character, so the total length of the
 * strings bounds their number. */
char **splitWordsBySeparator(char **words, int wordsSize, char separator,
                             int *returnSize)
{
    char delim[2] = {separator, '\0'};
    char *copy, *tok, **out;
    size_t cap = 1;
    int m = 0, w;

    for (w = 0; w < wordsSize; w++)
        cap += strlen(words[w]);
    out = malloc(cap * sizeof *out);
    for (w = 0; w < wordsSize; w++) {
        copy = malloc(strlen(words[w]) + 1);
        strcpy(copy, words[w]);
        for (tok = strtok(copy, delim); tok; tok = strtok(NULL, delim)) {
            out[m] = malloc(strlen(tok) + 1);
            strcpy(out[m], tok);
            m++;
        }
        free(copy);
    }
    *returnSize = m;
    return out;
}
