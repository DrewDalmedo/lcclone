/* Reference solution: count each letter in a 26-entry table, then compare
 * every nonzero count with the count of s[0], a letter that surely occurs. */
bool areOccurrencesEqual(char *s)
{
    int count[26] = {0};
    int i, target;

    for (i = 0; s[i] != '\0'; i++)
        count[s[i] - 'a']++;
    target = count[s[0] - 'a'];
    for (i = 0; i < 26; i++)
        if (count[i] != 0 && count[i] != target)
            return false;
    return true;
}
