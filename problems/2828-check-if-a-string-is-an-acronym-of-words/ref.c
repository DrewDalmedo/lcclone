/* Reference solution: s needs one letter per word, so compare the lengths
 * first, then each letter of s with the first letter of its word. */
bool isAcronym(char **words, int wordsSize, char *s)
{
    int i;

    if (strlen(s) != (size_t)wordsSize)
        return false;
    for (i = 0; i < wordsSize; i++)
        if (s[i] != words[i][0])
            return false;
    return true;
}
