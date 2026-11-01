/* Slow and obviously right, for short strings only: list every
 * subsequence of a and of b (one for each set of positions), and keep the
 * longest that is not also a subsequence of the other string. */
static int is_subsequence(const char *t, const char *s)
{
    while (*t && *s)
        if (*t == *s++)
            t++;
    return *t == '\0';
}

/* The longest subsequence of s that is not one of other, or -1. */
static int longest_not_in(const char *s, const char *other)
{
    int n = (int)strlen(s), best = -1, len, i;
    unsigned long mask;
    char t[32];

    for (mask = 0; mask < 1ul << n; mask++) {
        len = 0;
        for (i = 0; i < n; i++)
            if (mask >> i & 1)
                t[len++] = s[i];
        t[len] = '\0';
        if (len > best && !is_subsequence(t, other))
            best = len;
    }
    return best;
}

int findLUSlength(char *a, char *b)
{
    int x = longest_not_in(a, b), y = longest_not_in(b, a);

    return x > y ? x : y;
}
