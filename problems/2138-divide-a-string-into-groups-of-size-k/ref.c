/* Reference solution: there are ceil(n / k) rows; cell j of row r holds
 * s[r * k + j] while that index is inside s, and fill after it. */
char **divideString(char *s, int k, char fill, int *returnSize)
{
    int n = (int)strlen(s), rows = (n + k - 1) / k, r, j, at;
    char **out = malloc((size_t)rows * sizeof *out);

    for (r = 0; r < rows; r++) {
        out[r] = malloc((size_t)k + 1);
        for (j = 0; j < k; j++) {
            at = r * k + j;
            out[r][j] = at < n ? s[at] : fill;
        }
        out[r][k] = '\0';
    }
    *returnSize = rows;
    return out;
}
