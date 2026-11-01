/* Slow and obviously right: cut text into its words, then try every gap
 * width from s down to 0 and take the first whose gaps fit in the s spaces;
 * build the line from the words and that width, and pad it with spaces up
 * to the length of text. */
char *reorderSpaces(char *text)
{
    char words[100][101];
    int n = (int)strlen(text), w = 0, s = 0, len = 0, i, k, g;
    char *out = malloc((size_t)n + 1);

    i = 0;
    while (i < n) {
        if (text[i] == ' ') {
            s++;
            i++;
            continue;
        }
        k = 0;
        while (i < n && text[i] != ' ')
            words[w][k++] = text[i++];
        words[w][k] = '\0';
        w++;
    }
    g = 0;
    if (w > 1)
        for (g = s; g * (w - 1) > s; g--)
            ;
    for (i = 0; i < w; i++) {
        if (i > 0)
            for (k = 0; k < g; k++)
                out[len++] = ' ';
        for (k = 0; words[i][k]; k++)
            out[len++] = words[i][k];
    }
    while (len < n)
        out[len++] = ' ';
    out[len] = '\0';
    return out;
}
