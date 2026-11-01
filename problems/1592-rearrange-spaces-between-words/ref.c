/* Reference solution: one pass counts the spaces and the words, then the
 * words are copied in order into a new buffer of the same length, with
 * s / (w - 1) spaces between neighbours and the rest of the spaces after
 * the last word (all of them when there is only one word). */
char *reorderSpaces(char *text)
{
    int n = (int)strlen(text), spaces = 0, words = 0, gap, i, len = 0;
    char *out = malloc((size_t)n + 1);

    for (i = 0; i < n; i++) {
        if (text[i] == ' ')
            spaces++;
        else if (i == 0 || text[i - 1] == ' ')
            words++;
    }
    gap = words > 1 ? spaces / (words - 1) : 0;
    for (i = 0; i < n; i++) {
        if (text[i] == ' ')
            continue;
        /* The first letter of a word other than the first: a gap first. */
        if (len > 0 && text[i - 1] == ' ') {
            memset(out + len, ' ', (size_t)gap);
            len += gap;
        }
        out[len++] = text[i];
    }
    memset(out + len, ' ', (size_t)(n - len));
    out[n] = '\0';
    return out;
}
