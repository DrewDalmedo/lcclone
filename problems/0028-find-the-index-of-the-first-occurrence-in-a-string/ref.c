/* Reference solution: Knuth-Morris-Pratt. fail[j] is the length of the
 * longest proper prefix of needle[0..j] that is also a suffix of it. After
 * a mismatch the search falls back to that prefix instead of starting
 * over, so every letter of haystack is read once: O(n + m) time. */
int strStr(char *haystack, char *needle)
{
    int n = (int)strlen(haystack), m = (int)strlen(needle), i, j;
    int *fail;

    if (m > n)
        return -1;
    fail = malloc((size_t)m * sizeof *fail);
    fail[0] = 0;
    for (i = 1, j = 0; i < m; i++) {
        while (j > 0 && needle[i] != needle[j])
            j = fail[j - 1];
        if (needle[i] == needle[j])
            j++;
        fail[i] = j;
    }
    for (i = 0, j = 0; i < n; i++) {
        while (j > 0 && haystack[i] != needle[j])
            j = fail[j - 1];
        if (haystack[i] == needle[j])
            j++;
        if (j == m) {
            free(fail);
            return i - m + 1;
        }
    }
    free(fail);
    return -1;
}
