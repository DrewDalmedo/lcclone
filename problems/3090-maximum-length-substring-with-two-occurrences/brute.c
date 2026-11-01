/* Slow and obviously right: count the letters of every piece anew. */
int maximumLengthSubstring(char *s)
{
    int n = (int)strlen(s), best = 0, i, j, t, ok;
    int count[26];

    for (i = 0; i < n; i++)
        for (j = i; j < n; j++) {
            memset(count, 0, sizeof count);
            ok = 1;
            for (t = i; t <= j; t++)
                if (++count[s[t] - 'a'] > 2)
                    ok = 0;
            if (ok && j - i + 1 > best)
                best = j - i + 1;
        }
    return best;
}
