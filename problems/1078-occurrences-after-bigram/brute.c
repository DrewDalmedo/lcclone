/* Slow and obviously right, without splitting: at every position of text
 * where a word starts, test whether text reads first, a space, second and
 * a space from there, and if so copy out the word that follows. */
char **findOcurrences(char *text, char *first, char *second, int *returnSize)
{
    size_t len = strlen(text), f = strlen(first), s = strlen(second);
    size_t p, q, e;
    char **out = malloc((len + 1) * sizeof *out);
    int m = 0;

    for (p = 0; p < len; p++) {
        if (p > 0 && text[p - 1] != ' ')
            continue; /* not the start of a word */
        q = p;
        if (len - q < f + 1 || strncmp(text + q, first, f) != 0 || text[q + f] != ' ')
            continue;
        q += f + 1;
        if (len - q < s + 1 || strncmp(text + q, second, s) != 0 || text[q + s] != ' ')
            continue;
        q += s + 1;
        for (e = q; e < len && text[e] != ' '; e++)
            ;
        out[m] = malloc(e - q + 1);
        memcpy(out[m], text + q, e - q);
        out[m][e - q] = '\0';
        m++;
    }
    *returnSize = m;
    return out;
}
