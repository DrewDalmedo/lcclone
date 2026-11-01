/* Slow and obviously right: for every start, count how many times each
 * letter occurs among the three characters there, and call the place good
 * when no letter occurs more than once. */
int countGoodSubstrings(char *s)
{
    int n = (int)strlen(s), count = 0, start, k, seen[26], good;

    for (start = 0; start + 3 <= n; start++) {
        memset(seen, 0, sizeof seen);
        good = 1;
        for (k = start; k < start + 3; k++)
            if (++seen[s[k] - 'a'] > 1)
                good = 0;
        count += good;
    }
    return count;
}
