/* Reference solution: word number i grows by its bleat, i + 2 letters, so
 * the result has strlen(sentence) + 2w + w(w + 1)/2 characters for w
 * words. Allocate that, then copy the words over one by one, moving the
 * first letter of each word that starts with a consonant. */
static int is_vowel(char c)
{
    switch (c) {
    case 'a': case 'e': case 'i': case 'o': case 'u':
    case 'A': case 'E': case 'I': case 'O': case 'U':
        return 1;
    default:
        return 0;
    }
}

char *toGoatLatin(char *sentence)
{
    size_t n = strlen(sentence), words = 1, len = 0, start, end, i, k;
    char *out;

    for (i = 0; i < n; i++)
        if (sentence[i] == ' ')
            words++;
    out = malloc(n + 2 * words + words * (words + 1) / 2 + 1);
    for (start = 0, i = 1; start < n; start = end + 1, i++) {
        for (end = start; end < n && sentence[end] != ' '; end++)
            ;
        if (i > 1)
            out[len++] = ' ';
        if (is_vowel(sentence[start])) {
            memcpy(out + len, sentence + start, end - start);
            len += end - start;
        } else {
            memcpy(out + len, sentence + start + 1, end - start - 1);
            len += end - start - 1;
            out[len++] = sentence[start];
        }
        out[len++] = 'm';
        for (k = 0; k < i + 1; k++)
            out[len++] = 'a';
    }
    out[len] = '\0';
    return out;
}
