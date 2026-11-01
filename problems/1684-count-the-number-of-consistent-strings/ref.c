/* Reference solution: turn allowed into a 26-bit mask of its letters, then
 * count the words in which no letter falls outside the mask. */
int countConsistentStrings(char *allowed, char **words, int wordsSize)
{
    int mask = 0, count = 0, i;
    const char *p;

    for (p = allowed; *p; p++)
        mask |= 1 << (*p - 'a');
    for (i = 0; i < wordsSize; i++) {
        for (p = words[i]; *p; p++)
            if (!(mask >> (*p - 'a') & 1))
                break;
        if (!*p)
            count++;
    }
    return count;
}
