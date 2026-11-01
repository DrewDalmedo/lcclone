/* Reference solution: split a copy of text into its words, then check
 * every pair of neighbouring words against first and second, copying the
 * word after each matching pair into the result. */
char **findOcurrences(char *text, char *first, char *second, int *returnSize)
{
    size_t len = strlen(text), wl;
    char *buf = malloc(len + 1), *p = buf;
    char **words = malloc((len / 2 + 1) * sizeof *words); /* at most that many */
    char **out;
    int n = 0, m = 0, i;

    memcpy(buf, text, len + 1);
    for (;;) {
        words[n++] = p;
        while (*p && *p != ' ')
            p++;
        if (!*p)
            break;
        *p++ = '\0';
    }
    out = malloc((size_t)(n > 2 ? n - 2 : 1) * sizeof *out);
    for (i = 0; i + 2 < n; i++)
        if (strcmp(words[i], first) == 0 && strcmp(words[i + 1], second) == 0) {
            wl = strlen(words[i + 2]) + 1;
            out[m] = malloc(wl);
            memcpy(out[m], words[i + 2], wl);
            m++;
        }
    free(words);
    free(buf);
    *returnSize = m;
    return out;
}
