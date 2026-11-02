/* A different method: after n - 2 folds, the first digit left is the sum
 * of C(n - 2, i) * s[i] and the second the sum of C(n - 2, i) * s[i + 1],
 * both mod 10. Build row n - 2 of Pascal's triangle mod 10 by additions
 * alone (the binomials themselves overflow) and compare the two sums. */
bool hasSameDigits(char *s)
{
    int n = (int)strlen(s), m = n - 2, r, i, left = 0, right = 0;
    int *row = malloc((size_t)(m + 1) * sizeof *row);

    row[0] = 1;
    for (r = 1; r <= m; r++) {
        row[r] = 1;
        for (i = r - 1; i > 0; i--)
            row[i] = (row[i] + row[i - 1]) % 10;
    }
    for (i = 0; i <= m; i++) {
        left = (left + row[i] * (s[i] - '0')) % 10;
        right = (right + row[i] * (s[i + 1] - '0')) % 10;
    }
    free(row);
    return left == right;
}
