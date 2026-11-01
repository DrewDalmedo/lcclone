/* Reference solution: the prefix function (the failure table of
 * Knuth-Morris-Pratt). fail[i] is the length of the longest proper prefix
 * of s[0..i] that is also a suffix of it. With L = fail[n - 1], p = n - L
 * is the shortest period of s (s[i] == s[i - p] for every i >= p), and s is
 * a repetition exactly when L > 0 and p divides n: any block length that
 * works is a multiple of p. O(n) time. */
bool repeatedSubstringPattern(char *s)
{
    int n = (int)strlen(s), i, j;
    int *fail = malloc((size_t)n * sizeof *fail);
    bool ok;

    fail[0] = 0;
    for (i = 1, j = 0; i < n; i++) {
        while (j > 0 && s[i] != s[j])
            j = fail[j - 1];
        if (s[i] == s[j])
            j++;
        fail[i] = j;
    }
    ok = fail[n - 1] > 0 && n % (n - fail[n - 1]) == 0;
    free(fail);
    return ok;
}
