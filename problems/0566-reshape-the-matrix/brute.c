/* Slow and obviously right: count mat's cells one by one. If the new shape
 * has as many, fill each of its cells (i, j) by walking through mat in
 * reading order until the (i * c + j)-th value (counting from 0); otherwise
 * copy mat cell by cell. */
int **matrixReshape(int **mat, int matSize, int *matColSize, int r, int c,
                    int *returnSize, int **returnColumnSizes)
{
    int count = 0, i, j, x, y, k, want;
    int **out;

    for (i = 0; i < matSize; i++)
        count += matColSize[i];
    if (r * c != count) {
        out = malloc((size_t)matSize * sizeof *out);
        *returnColumnSizes = malloc((size_t)matSize * sizeof **returnColumnSizes);
        for (i = 0; i < matSize; i++) {
            out[i] = malloc((size_t)matColSize[i] * sizeof **out);
            (*returnColumnSizes)[i] = matColSize[i];
            for (j = 0; j < matColSize[i]; j++)
                out[i][j] = mat[i][j];
        }
        *returnSize = matSize;
        return out;
    }
    out = malloc((size_t)r * sizeof *out);
    *returnColumnSizes = malloc((size_t)r * sizeof **returnColumnSizes);
    for (i = 0; i < r; i++) {
        out[i] = malloc((size_t)c * sizeof **out);
        (*returnColumnSizes)[i] = c;
        for (j = 0; j < c; j++) {
            want = i * c + j;
            k = 0;
            for (x = 0; x < matSize; x++)
                for (y = 0; y < matColSize[x]; y++) {
                    if (k == want)
                        out[i][j] = mat[x][y];
                    k++;
                }
        }
    }
    *returnSize = r;
    return out;
}
