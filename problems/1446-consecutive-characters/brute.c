/* Slow and obviously right: try every length from the longest down, and
 * every substring of that length, until one is all a single letter. */
int maxPower(char *s)
{
    int n = (int)strlen(s), len, start, i, same;

    for (len = n; len > 1; len--)
        for (start = 0; start + len <= n; start++) {
            same = 1;
            for (i = start + 1; i < start + len; i++)
                if (s[i] != s[start])
                    same = 0;
            if (same)
                return len;
        }
    return 1;
}
