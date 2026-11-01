/* Reference solution: number the m * n values 0, 1, ... in reading order.
 * Value k sits at row k / n, column k % n of mat, and goes to row k / c,
 * column k % c of the result. When r * c != m * n, the result is mat's own
 * shape, so the same loop makes a copy of mat. */
int **matrixReshape(int **mat, int matSize, int *matColSize, int r, int c,
                    int *returnSize, int **returnColumnSizes)
{
    int m = matSize, n = matColSize[0], i, k;
    int **out;

    if (r * c != m * n) {
        r = m;
        c = n;
    }
    out = malloc((size_t)r * sizeof *out);
    *returnColumnSizes = malloc((size_t)r * sizeof **returnColumnSizes);
    for (i = 0; i < r; i++) {
        out[i] = malloc((size_t)c * sizeof **out);
        (*returnColumnSizes)[i] = c;
    }
    for (k = 0; k < m * n; k++)
        out[k / c][k % c] = mat[k / n][k % n];
    *returnSize = r;
    return out;
}
