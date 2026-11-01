/* Slow and obviously right: for every block length d that divides n, other
 * than n itself, write the first d letters out n / d times and compare the
 * result with s. */
bool repeatedSubstringPattern(char *s)
{
    int n = (int)strlen(s), d, i;
    char *t = malloc((size_t)n + 1);
    bool found = false;

    for (d = 1; d < n && !found; d++) {
        if (n % d != 0)
            continue;
        for (i = 0; i < n; i++)
            t[i] = s[i % d];
        t[n] = '\0';
        if (strcmp(t, s) == 0)
            found = true;
    }
    free(t);
    return found;
}
