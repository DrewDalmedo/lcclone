/* Slow and obviously right: list every edge once, as a pair i < j with a 1
 * in the upper triangle, and credit it to both of its ends. */
int *findDegrees(int **matrix, int matrixSize, int *matrixColSize,
                 int *returnSize)
{
    int n = matrixSize, i, j;
    int *deg = calloc((size_t)n, sizeof *deg);

    (void)matrixColSize;
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (matrix[i][j] == 1) {
                deg[i]++;
                deg[j]++;
            }
    *returnSize = n;
    return deg;
}
