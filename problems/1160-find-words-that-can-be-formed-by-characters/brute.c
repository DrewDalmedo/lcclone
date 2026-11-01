/* Slow and obviously right: for each word, take a fresh copy of chars and,
 * letter by letter, look for a piece not used yet, crossing it out when
 * found. The word counts when every letter finds one. */
int countCharacters(char **words, int wordsSize, char *chars)
{
    char pool[101];
    int total = 0, i, j, k, ok;

    for (i = 0; i < wordsSize; i++) {
        strcpy(pool, chars);
        ok = 1;
        for (j = 0; words[i][j] && ok; j++) {
            for (k = 0; pool[k] && pool[k] != words[i][j]; k++)
                ;
            if (pool[k])
                pool[k] = '#';
            else
                ok = 0;
        }
        if (ok)
            total += (int)strlen(words[i]);
    }
    return total;
}
