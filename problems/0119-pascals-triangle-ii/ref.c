/* Reference solution: C(n, j) = C(n, j - 1) * (n - j + 1) / j, starting
 * from C(n, 0) = 1. The division is always exact, but for n = 33 the
 * product before it reaches 19835652870, so it is done in 64 bits. */
int *getRow(int rowIndex, int *returnSize)
{
    int *row = malloc((size_t)(rowIndex + 1) * sizeof *row);
    long long c = 1;
    int j;

    row[0] = 1;
    for (j = 1; j <= rowIndex; j++) {
        c = c * (rowIndex - j + 1) / j;
        row[j] = (int)c;
    }
    *returnSize = rowIndex + 1;
    return row;
}
