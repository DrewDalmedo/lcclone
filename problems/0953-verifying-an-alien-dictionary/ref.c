/* Reference solution: give each letter its position in order, then check
 * each pair of neighbours at their first differing letter, or by length
 * when one word is a prefix of the other. */
bool isAlienSorted(char **words, int wordsSize, char *order)
{
    int rank[26], i, k;
    const char *a, *b;

    for (i = 0; i < 26; i++)
        rank[order[i] - 'a'] = i;
    for (i = 0; i + 1 < wordsSize; i++) {
        a = words[i];
        b = words[i + 1];
        for (k = 0; a[k] && a[k] == b[k]; k++)
            ;
        /* a[k] differs from b[k], or a ended (then a <= b). */
        if (a[k] && (!b[k] || rank[a[k] - 'a'] > rank[b[k] - 'a']))
            return false;
    }
    return true;
}
