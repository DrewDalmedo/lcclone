/* Slow and obviously right: check every substring of even length against
 * the definition (first half all 0s, second half all 1s) and keep the
 * longest that passes. */
int findTheLongestBalancedSubstring(char *s)
{
    int n = (int)strlen(s), best = 0, i, len, j, ok;

    for (i = 0; i < n; i++)
        for (len = 2; i + len <= n; len += 2) {
            ok = 1;
            for (j = 0; j < len; j++)
                if (s[i + j] != (j < len / 2 ? '0' : '1'))
                    ok = 0;
            if (ok && len > best)
                best = len;
        }
    return best;
}
