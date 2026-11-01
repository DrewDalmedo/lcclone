/* Reference solution: search word for each pattern with strstr, and count
 * the patterns it finds. Each entry is searched on its own, so equal
 * entries are each counted, and each counts once however often it occurs. */
int numOfStrings(char **patterns, int patternsSize, char *word)
{
    int count = 0, i;

    for (i = 0; i < patternsSize; i++)
        if (strstr(word, patterns[i]) != NULL)
            count++;
    return count;
}
