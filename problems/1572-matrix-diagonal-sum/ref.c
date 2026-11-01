/* Reference solution: one pass over the rows, adding the cell of each
 * diagonal in that row. For odd n the middle cell was added twice, so
 * take it off once. */
int diagonalSum(int **mat, int matSize, int *matColSize)
{
    int n = matSize, sum = 0, i;

    (void)matColSize;
    for (i = 0; i < n; i++)
        sum += mat[i][i] + mat[i][n - 1 - i];
    if (n % 2 == 1)
        sum -= mat[n / 2][n / 2];
    return sum;
}
