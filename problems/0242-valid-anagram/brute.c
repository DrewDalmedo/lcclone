/* Slow and obviously right: pair every letter of s with an unused equal
 * letter of t, and check that no letter of t is left over. */
bool isAnagram(char *s, char *t)
{
    size_t n = strlen(s), m = strlen(t), i, k;
    bool *used = calloc(m + 1, sizeof *used), ok = true;

    for (i = 0; i < n && ok; i++) {
        for (k = 0; k < m && (used[k] || t[k] != s[i]); k++)
            ;
        if (k == m)
            ok = false;
        else
            used[k] = true;
    }
    for (k = 0; k < m && ok; k++)
        if (!used[k])
            ok = false;
    free(used);
    return ok;
}
