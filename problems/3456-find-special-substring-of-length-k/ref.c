/* Reference solution: walk along s block by block, measuring each
 * maximal run of one letter, and report whether some run is exactly k
 * long. */
bool hasSpecialSubstring(char *s, int k)
{
    int i = 0, j;

    while (s[i]) {
        j = i;
        while (s[j] == s[i])
            j++;
        if (j - i == k)
            return true;
        i = j;
    }
    return false;
}
