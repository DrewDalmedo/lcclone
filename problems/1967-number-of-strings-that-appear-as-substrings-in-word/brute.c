/* Slow and obviously right: compare each pattern, letter by letter, with
 * every piece word[a..b] of word, and count it if one of them matches. */
int numOfStrings(char **patterns, int patternsSize, char *word)
{
    int w = (int)strlen(word), count = 0, i, a, b, k, len, found;

    for (i = 0; i < patternsSize; i++) {
        len = (int)strlen(patterns[i]);
        found = 0;
        for (a = 0; a < w; a++)
            for (b = a; b < w; b++) {
                if (b - a + 1 != len)
                    continue;
                for (k = 0; k < len && word[a + k] == patterns[i][k]; k++)
                    ;
                if (k == len)
                    found = 1;
            }
        count += found;
    }
    return count;
}
