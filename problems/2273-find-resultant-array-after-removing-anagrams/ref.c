/* Reference solution: one pass. A word stays exactly when it is not an
 * anagram of the word just before it in the original list: if that word
 * was struck, it was an anagram of the word kept before it, and being
 * anagrams is transitive. Anagrams are compared by their 26 letter counts,
 * without touching the words, and the words kept are copied. */
static int anagrams(const char *a, const char *b)
{
    int count[26] = {0}, i;

    if (strlen(a) != strlen(b))
        return 0;
    for (; *a; a++, b++) {
        count[*a - 'a']++;
        count[*b - 'a']--;
    }
    for (i = 0; i < 26; i++)
        if (count[i] != 0)
            return 0;
    return 1;
}

char **removeAnagrams(char **words, int wordsSize, int *returnSize)
{
    char **out = malloc((size_t)wordsSize * sizeof *out);
    size_t len;
    int m = 0, i;

    for (i = 0; i < wordsSize; i++)
        if (i == 0 || !anagrams(words[i - 1], words[i])) {
            len = strlen(words[i]) + 1;
            out[m] = malloc(len);
            memcpy(out[m], words[i], len);
            m++;
        }
    *returnSize = m;
    return out;
}
