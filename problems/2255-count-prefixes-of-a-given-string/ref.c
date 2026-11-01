/* Reference solution: a word is a prefix of s when it is no longer than s
 * and its letters match the first letters of s one by one. Checking the
 * length first means no comparison reads past the end of s. */
int countPrefixes(char **words, int wordsSize, char *s)
{
    size_t n = strlen(s), len;
    int count = 0, i;

    for (i = 0; i < wordsSize; i++) {
        len = strlen(words[i]);
        if (len <= n && memcmp(words[i], s, len) == 0)
            count++;
    }
    return count;
}
