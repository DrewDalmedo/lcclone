/* Reference solution: there are only two alternating strings of each
 * length, one starting with 0 and one starting with 1, and every position
 * differs from exactly one of them. Count the positions c that differ from
 * the one starting with 0; the other needs the remaining n - c changes. */
int minOperations(char *s)
{
    int n = (int)strlen(s), c = 0, i;

    for (i = 0; i < n; i++)
        if (s[i] != '0' + i % 2)
            c++;
    return c < n - c ? c : n - c;
}
