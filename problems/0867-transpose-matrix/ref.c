/* Reference solution: build the n x m result column by column. */
int **transpose(int **matrix, int matrixSize, int *matrixColSize,
                int *returnSize, int **returnColumnSizes)
{
    int m = matrixSize, n = matrixColSize[0], i, j;
    int **out = malloc((size_t)n * sizeof *out);

    *returnColumnSizes = malloc((size_t)n * sizeof **returnColumnSizes);
    for (j = 0; j < n; j++) {
        out[j] = malloc((size_t)m * sizeof **out);
        (*returnColumnSizes)[j] = m;
        for (i = 0; i < m; i++)
            out[j][i] = matrix[i][j];
    }
    *returnSize = n;
    return out;
}
