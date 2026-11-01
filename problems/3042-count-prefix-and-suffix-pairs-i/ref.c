/* Reference solution: try every pair i < j. words[i] bookends words[j]
 * when it is no longer, and it matches both the first and the last
 * strlen(words[i]) letters of words[j]. */
int countPrefixSuffixPairs(char **words, int wordsSize)
{
    size_t len[50], la, lb;
    int count = 0, i, j;

    for (i = 0; i < wordsSize; i++)
        len[i] = strlen(words[i]);
    for (j = 1; j < wordsSize; j++) {
        lb = len[j];
        for (i = 0; i < j; i++) {
            la = len[i];
            if (la <= lb && memcmp(words[j], words[i], la) == 0 &&
                memcmp(words[j] + (lb - la), words[i], la) == 0)
                count++;
        }
    }
    return count;
}
