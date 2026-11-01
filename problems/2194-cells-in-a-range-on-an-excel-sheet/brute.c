/* Slow and obviously right: go through all 234 cells of the sheet row by
 * row, keep those inside the block, then sort the kept names by column
 * and, within a column, by row. */
static int by_column_then_row(const void *a, const void *b)
{
    const char *x = *(char *const *)a, *y = *(char *const *)b;

    if (x[0] != y[0])
        return x[0] - y[0];
    return x[1] - y[1];
}

char **cellsInRange(char *s, int *returnSize)
{
    char **out = malloc(26 * 9 * sizeof *out);
    int m = 0, row, col;

    for (row = 1; row <= 9; row++)
        for (col = 0; col < 26; col++) {
            char letter = (char)('A' + col), digit = (char)('0' + row);

            if (letter >= s[0] && letter <= s[3] && digit >= s[1] &&
                digit <= s[4]) {
                out[m] = malloc(3);
                out[m][0] = letter;
                out[m][1] = digit;
                out[m][2] = '\0';
                m++;
            }
        }
    qsort(out, (size_t)m, sizeof *out, by_column_then_row);
    *returnSize = m;
    return out;
}
