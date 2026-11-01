/* Slow and obviously right: for every blank, scan its whole column again
 * for the largest value. */
int **modifiedMatrix(int **matrix, int matrixSize, int *matrixColSize,
                     int *returnSize, int **returnColumnSizes)
{
    int m = matrixSize, i, j, k, best;
    int **out = malloc((size_t)m * sizeof *out);

    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        int n = matrixColSize[i];

        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++) {
            if (matrix[i][j] != -1) {
                out[i][j] = matrix[i][j];
                continue;
            }
            best = matrix[0][j];
            for (k = 1; k < m; k++)
                if (matrix[k][j] > best)
                    best = matrix[k][j];
            out[i][j] = best;
        }
    }
    *returnSize = m;
    return out;
}
