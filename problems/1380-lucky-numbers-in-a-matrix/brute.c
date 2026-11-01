/* Slow and obviously right: compare every number with every other number
 * of its row and of its column. */
int *luckyNumbers(int **matrix, int matrixSize, int *matrixColSize,
                  int *returnSize)
{
    int m = matrixSize, n = matrixColSize[0], i, j, k, lucky;
    int *out = malloc((size_t)m * n * sizeof *out);

    *returnSize = 0;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++) {
            lucky = 1;
            for (k = 0; k < n; k++)
                if (matrix[i][k] < matrix[i][j])
                    lucky = 0;
            for (k = 0; k < m; k++)
                if (matrix[k][j] > matrix[i][j])
                    lucky = 0;
            if (lucky)
                out[(*returnSize)++] = matrix[i][j];
        }
    return out;
}
