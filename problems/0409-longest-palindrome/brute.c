/* Slow and obviously right: pair the tiles off one pair at a time, each
 * unused tile with the next unused equal tile after it. The pairs make up
 * the two halves of the row, and one unpaired tile, if there is any, goes
 * in the middle. */
int longestPalindrome(char *s)
{
    size_t n = strlen(s), i, k;
    char *used = calloc(n + 1, 1);
    int len = 0, middle = 0;

    for (i = 0; i < n; i++) {
        if (used[i])
            continue;
        for (k = i + 1; k < n && (used[k] || s[k] != s[i]); k++)
            ;
        if (k < n) {
            used[i] = used[k] = 1;
            len += 2;
        } else {
            middle = 1;
        }
    }
    free(used);
    return len + middle;
}
