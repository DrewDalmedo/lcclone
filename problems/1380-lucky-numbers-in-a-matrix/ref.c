/* Reference solution: one pass finds the smallest number of every row and
 * the largest of every column; a number equal to both is lucky. */
int *luckyNumbers(int **matrix, int matrixSize, int *matrixColSize,
                  int *returnSize)
{
    int m = matrixSize, n = matrixColSize[0], i, j;
    int *rowMin = malloc((size_t)m * sizeof *rowMin);
    int *colMax = malloc((size_t)n * sizeof *colMax);
    int *out = malloc((size_t)m * sizeof *out);

    for (i = 0; i < m; i++)
        rowMin[i] = matrix[i][0];
    for (j = 0; j < n; j++)
        colMax[j] = matrix[0][j];
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++) {
            if (matrix[i][j] < rowMin[i])
                rowMin[i] = matrix[i][j];
            if (matrix[i][j] > colMax[j])
                colMax[j] = matrix[i][j];
        }
    *returnSize = 0;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (matrix[i][j] == rowMin[i] && matrix[i][j] == colMax[j])
                out[(*returnSize)++] = matrix[i][j];
    free(rowMin);
    free(colMax);
    return out;
}
