/* Slow and obviously right: write each word out backwards and compare the
 * result with the word; return a copy of the first word that is unchanged
 * by the reversal, or an empty string. */
char *firstPalindrome(char **words, int wordsSize)
{
    char back[101], *out;
    size_t len, k;
    int i;

    for (i = 0; i < wordsSize; i++) {
        len = strlen(words[i]);
        for (k = 0; k < len; k++)
            back[k] = words[i][len - 1 - k];
        back[len] = '\0';
        if (strcmp(back, words[i]) == 0) {
            out = malloc(len + 1);
            strcpy(out, words[i]);
            return out;
        }
    }
    out = malloc(1);
    out[0] = '\0';
    return out;
}
