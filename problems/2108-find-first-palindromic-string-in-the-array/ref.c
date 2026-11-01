/* Reference solution: test the words in order, comparing letters from
 * both ends towards the middle, and return a copy of the first word that
 * passes, or an empty string when none does. */
static int is_palindrome(const char *w)
{
    size_t i = 0, j = strlen(w);

    while (i + 1 < j) {
        if (w[i] != w[j - 1])
            return 0;
        i++;
        j--;
    }
    return 1;
}

char *firstPalindrome(char **words, int wordsSize)
{
    char *out;
    size_t len;
    int i;

    for (i = 0; i < wordsSize; i++)
        if (is_palindrome(words[i])) {
            len = strlen(words[i]);
            out = malloc(len + 1);
            memcpy(out, words[i], len + 1);
            return out;
        }
    out = malloc(1);
    out[0] = '\0';
    return out;
}
