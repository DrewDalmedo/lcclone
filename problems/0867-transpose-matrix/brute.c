/* Slow and obviously right: read the matrix row by row, and add each
 * number to the end of the output row named by its column. */
int **transpose(int **matrix, int matrixSize, int *matrixColSize,
                int *returnSize, int **returnColumnSizes)
{
    int rows = matrixColSize[0], i, j;
    int **out = malloc((size_t)rows * sizeof *out);
    int *len = calloc((size_t)rows, sizeof *len);

    for (j = 0; j < rows; j++)
        out[j] = malloc((size_t)matrixSize * sizeof **out);
    for (i = 0; i < matrixSize; i++)
        for (j = 0; j < matrixColSize[i]; j++)
            out[j][len[j]++] = matrix[i][j];
    *returnSize = rows;
    *returnColumnSizes = len;
    return out;
}
