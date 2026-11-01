/* Reference solution: count each letter of s once, then return the first
 * position whose letter was counted only once. */
int firstUniqChar(char *s)
{
    int count[26] = {0}, i;

    for (i = 0; s[i]; i++)
        count[s[i] - 'a']++;
    for (i = 0; s[i]; i++)
        if (count[s[i] - 'a'] == 1)
            return i;
    return -1;
}
