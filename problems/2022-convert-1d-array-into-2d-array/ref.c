/* Reference solution: the shape works only when m * n equals the length
 * of original (m * n is at most 1.6 * 10^9, which fits in an int), so
 * check that before allocating anything; then copy original row by row,
 * n numbers to a row. */
int **construct2DArray(int *original, int originalSize, int m, int n,
                       int *returnSize, int **returnColumnSizes)
{
    int **out, r;

    if (m * n != originalSize) {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }
    out = malloc((size_t)m * sizeof *out);
    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (r = 0; r < m; r++) {
        out[r] = malloc((size_t)n * sizeof **out);
        memcpy(out[r], original + (size_t)r * n, (size_t)n * sizeof **out);
        (*returnColumnSizes)[r] = n;
    }
    *returnSize = m;
    return out;
}
