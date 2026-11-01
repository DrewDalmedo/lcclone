/* Slow and obviously right: for every position, count anew how often its
 * character occurs in s, and compare that with the same count for every
 * other position. */
static int occurrences(const char *s, int n, char c)
{
    int k = 0, i;

    for (i = 0; i < n; i++)
        if (s[i] == c)
            k++;
    return k;
}

bool areOccurrencesEqual(char *s)
{
    int n = (int)strlen(s), i, j;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (occurrences(s, n, s[i]) != occurrences(s, n, s[j]))
                return false;
    return true;
}
