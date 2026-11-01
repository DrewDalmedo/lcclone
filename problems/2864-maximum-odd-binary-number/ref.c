/* Reference solution: one '1' has to go last to make the number odd.
 * Every other '1' is worth most at the front, so the answer is the
 * remaining ones, then all the zeros, then that last '1'. Count the ones
 * and write the answer into a new buffer of n + 1 bytes. */
char *maximumOddBinaryNumber(char *s)
{
    size_t n = strlen(s), ones = 0, i;
    char *out = malloc(n + 1);

    for (i = 0; i < n; i++)
        if (s[i] == '1')
            ones++;
    for (i = 0; i < n; i++)
        out[i] = (i + 1 < ones || i == n - 1) ? '1' : '0';
    out[n] = '\0';
    return out;
}
