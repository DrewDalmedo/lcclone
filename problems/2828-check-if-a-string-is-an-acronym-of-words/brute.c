/* Slow and obviously right: write the initials out as a string of their
 * own, then compare that string with s as a whole. */
bool isAcronym(char **words, int wordsSize, char *s)
{
    char *initials = malloc((size_t)wordsSize + 1);
    bool same;
    int i;

    for (i = 0; i < wordsSize; i++)
        initials[i] = words[i][0];
    initials[wordsSize] = '\0';
    same = strcmp(initials, s) == 0;
    free(initials);
    return same;
}
