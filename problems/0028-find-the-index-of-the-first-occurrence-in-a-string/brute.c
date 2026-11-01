/* Slow and obviously right: at every index of haystack, compare needle
 * letter by letter, stopping at the end of haystack. */
int strStr(char *haystack, char *needle)
{
    int n = (int)strlen(haystack), m = (int)strlen(needle), i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m && i + j < n && haystack[i + j] == needle[j]; j++)
            ;
        if (j == m)
            return i;
    }
    return -1;
}
