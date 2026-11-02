/* Reference solution: copy the digits into an int array and fold it in
 * place, left to right, so that each new digit d[j] is made from the old
 * d[j] and d[j + 1] before d[j + 1] is overwritten. Reducing mod 10 at
 * every step keeps the values small. Stop when two digits are left. */
bool hasSameDigits(char *s)
{
    int n = (int)strlen(s), len, j;
    int *d = malloc((size_t)n * sizeof *d);
    bool same;

    for (j = 0; j < n; j++)
        d[j] = s[j] - '0';
    for (len = n; len > 2; len--)
        for (j = 0; j + 1 < len; j++)
            d[j] = (d[j] + d[j + 1]) % 10;
    same = d[0] == d[1];
    free(d);
    return same;
}
