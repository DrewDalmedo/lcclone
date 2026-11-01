/* Reference solution: find the largest value of each column in one pass,
 * then build a copy of the matrix with each -1 replaced by its column's
 * largest value. */
int **modifiedMatrix(int **matrix, int matrixSize, int *matrixColSize,
                     int *returnSize, int **returnColumnSizes)
{
    int m = matrixSize, n = matrixColSize[0], i, j;
    int *top = malloc((size_t)n * sizeof *top);
    int **out = malloc((size_t)m * sizeof *out);

    for (j = 0; j < n; j++)
        top[j] = -1;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (matrix[i][j] > top[j])
                top[j] = matrix[i][j];
    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++)
            out[i][j] = matrix[i][j] == -1 ? top[j] : matrix[i][j];
    }
    free(top);
    *returnSize = m;
    return out;
}
