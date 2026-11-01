/* Slow and obviously right: for each position from the left, count how
 * often its letter occurs in the whole string. */
int firstUniqChar(char *s)
{
    int n = (int)strlen(s), i, k, copies;

    for (i = 0; i < n; i++) {
        copies = 0;
        for (k = 0; k < n; k++)
            if (s[k] == s[i])
                copies++;
        if (copies == 1)
            return i;
    }
    return -1;
}
