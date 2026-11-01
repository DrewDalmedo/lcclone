/* Slow and obviously right: a star is outdoors when an even number of bars
 * come before it, so count the bars before each star anew. */
int countAsterisks(char *s)
{
    int n = (int)strlen(s), count = 0, bars, i, k;

    for (i = 0; i < n; i++) {
        if (s[i] != '*')
            continue;
        bars = 0;
        for (k = 0; k < i; k++)
            if (s[k] == '|')
                bars++;
        if (bars % 2 == 0)
            count++;
    }
    return count;
}
