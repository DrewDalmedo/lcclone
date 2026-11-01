/* Reference solution: one table of 26 signed differences, counting up for
 * each letter of word1 and down for each letter of word2; then every entry
 * must lie between -3 and 3. */
bool checkAlmostEquivalent(char *word1, char *word2)
{
    int diff[26] = {0}, i;

    for (i = 0; word1[i] != '\0'; i++)
        diff[word1[i] - 'a']++;
    for (i = 0; word2[i] != '\0'; i++)
        diff[word2[i] - 'a']--;
    for (i = 0; i < 26; i++)
        if (diff[i] > 3 || diff[i] < -3)
            return false;
    return true;
}
