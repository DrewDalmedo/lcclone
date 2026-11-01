/* Slow and obviously right: write out every prefix of s (its first letter,
 * its first two letters, and so on up to all of s) and count the words
 * equal to one of them. */
int countPrefixes(char **words, int wordsSize, char *s)
{
    char prefix[11][11];
    int n = (int)strlen(s), count = 0, i, k;

    for (k = 1; k <= n; k++) {
        memcpy(prefix[k], s, (size_t)k);
        prefix[k][k] = '\0';
    }
    for (i = 0; i < wordsSize; i++)
        for (k = 1; k <= n; k++)
            if (strcmp(words[i], prefix[k]) == 0)
                count++;
    return count;
}
