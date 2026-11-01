/* Slow and obviously right, for short strings only: go through every
 * palindrome of the same length in dictionary order, by counting through
 * its first half (middle letter included) from "aa...a" to "zz...z", and
 * keep the first one that needs the fewest edits, an edit being a position
 * where it differs from s. */
char *makeSmallestPalindrome(char *s)
{
    int n = (int)strlen(s), half = (n + 1) / 2, best = n + 1, edits, i;
    char *cand = malloc((size_t)n + 1), *out = malloc((size_t)n + 1);

    for (i = 0; i < half; i++)
        cand[i] = 'a';
    cand[n] = '\0';
    for (;;) {
        for (i = 0; i < half; i++)
            cand[n - 1 - i] = cand[i];
        edits = 0;
        for (i = 0; i < n; i++)
            edits += cand[i] != s[i];
        if (edits < best) {
            best = edits;
            memcpy(out, cand, (size_t)n + 1);
        }
        for (i = half - 1; i >= 0 && cand[i] == 'z'; i--)
            cand[i] = 'a';
        if (i < 0)
            break;
        cand[i]++;
    }
    free(cand);
    return out;
}
