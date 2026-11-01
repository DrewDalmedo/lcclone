/* Slow and obviously right: try every window of k characters, checking
 * that its letters are all equal and that the characters just outside it
 * (where there are any) differ from them. */
bool hasSpecialSubstring(char *s, int k)
{
    int n = (int)strlen(s), i, j, same;

    for (i = 0; i + k <= n; i++) {
        same = 1;
        for (j = i; j < i + k; j++)
            if (s[j] != s[i])
                same = 0;
        if (same && (i == 0 || s[i - 1] != s[i])
            && (i + k == n || s[i + k] != s[i]))
            return true;
    }
    return false;
}
