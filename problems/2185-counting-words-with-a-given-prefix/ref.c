/* Reference solution: compare the first strlen(pref) characters of each
 * word with pref. strncmp stops at the word's terminating NUL, which
 * differs from any letter of pref, so a word shorter than pref never
 * matches. O(total length) time. */
int prefixCount(char **words, int wordsSize, char *pref)
{
    size_t len = strlen(pref);
    int count = 0, i;

    for (i = 0; i < wordsSize; i++)
        if (strncmp(words[i], pref, len) == 0)
            count++;
    return count;
}
