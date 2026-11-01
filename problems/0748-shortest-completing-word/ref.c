/* Reference solution: count the plate's letters, folded to lower case, in
 * a table of 26. Then go through the words in order, and check only those
 * shorter than the best word so far: count the word's letters and compare
 * the two tables. The first completing word of the smallest length stays. */
char *shortestCompletingWord(char *licensePlate, char **words, int wordsSize)
{
    int need[26] = {0}, have[26], best = -1, i, k;
    size_t len, bestLen = 0;
    const char *p;
    char *out;

    for (p = licensePlate; *p; p++)
        if (*p >= 'a' && *p <= 'z')
            need[*p - 'a']++;
        else if (*p >= 'A' && *p <= 'Z')
            need[*p - 'A']++;
    for (i = 0; i < wordsSize; i++) {
        len = strlen(words[i]);
        if (best >= 0 && len >= bestLen)
            continue;
        memset(have, 0, sizeof have);
        for (p = words[i]; *p; p++)
            have[*p - 'a']++;
        for (k = 0; k < 26 && have[k] >= need[k]; k++)
            ;
        if (k == 26) {
            best = i;
            bestLen = len;
        }
    }
    out = malloc(bestLen + 1);
    memcpy(out, words[best], bestLen + 1);
    return out;
}
