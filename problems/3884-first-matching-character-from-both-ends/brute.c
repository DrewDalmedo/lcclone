/* Slow and obviously right: try every index of the string in order,
 * straight from the definition. */
int firstMatchingIndex(char *s)
{
    int n = (int)strlen(s), i;

    for (i = 0; i < n; i++)
        if (s[i] == s[n - 1 - i])
            return i;
    return -1;
}
