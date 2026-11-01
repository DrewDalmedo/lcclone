/* Reference solution: strings of different lengths can't match; otherwise
 * count each letter up for s and down for t, and every count must end at
 * zero. */
bool isAnagram(char *s, char *t)
{
    size_t n = strlen(s), i;
    int count[26] = {0}, c;

    if (strlen(t) != n)
        return false;
    for (i = 0; i < n; i++) {
        count[s[i] - 'a']++;
        count[t[i] - 'a']--;
    }
    for (c = 0; c < 26; c++)
        if (count[c] != 0)
            return false;
    return true;
}
