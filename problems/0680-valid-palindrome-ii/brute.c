/* Slow and obviously right: check s itself, then every string made by
 * deleting one character, comparing each with its reverse. */
static bool is_palindrome(const char *t, size_t m)
{
    char *rev = malloc(m + 1);
    size_t i;
    bool same;

    for (i = 0; i < m; i++)
        rev[i] = t[m - 1 - i];
    same = memcmp(rev, t, m) == 0;
    free(rev);
    return same;
}

bool validPalindrome(char *s)
{
    size_t n = strlen(s), del, i, m;
    char *t = malloc(n + 1);
    bool found = is_palindrome(s, n);

    for (del = 0; del < n && !found; del++) {
        m = 0;
        for (i = 0; i < n; i++)
            if (i != del)
                t[m++] = s[i];
        found = is_palindrome(t, m);
    }
    free(t);
    return found;
}
