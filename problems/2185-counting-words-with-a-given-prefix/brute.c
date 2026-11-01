/* Slow and obviously right: a word starts with pref when it is at least as
 * long as pref and agrees with it at every position of pref, checked one
 * letter at a time. */
int prefixCount(char **words, int wordsSize, char *pref)
{
    int plen = (int)strlen(pref), count = 0, i, k, same;

    for (i = 0; i < wordsSize; i++) {
        if ((int)strlen(words[i]) < plen)
            continue;
        same = 1;
        for (k = 0; k < plen; k++)
            if (words[i][k] != pref[k])
                same = 0;
        count += same;
    }
    return count;
}
