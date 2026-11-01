/* Slow and obviously right: look at every cell and add it once if it
 * lies on either diagonal. */
int diagonalSum(int **mat, int matSize, int *matColSize)
{
    int n = matSize, sum = 0, i, j;

    (void)matColSize;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (i == j || i + j == n - 1)
                sum += mat[i][j];
    return sum;
}
