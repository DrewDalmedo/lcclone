/* Slow and obviously right: fill the cells one at a time, row by row,
 * with the next unused number. Fail if the numbers run out before every
 * cell is filled, or if some are left over at the end. */
int **construct2DArray(int *original, int originalSize, int m, int n,
                       int *returnSize, int **returnColumnSizes)
{
    int **out = malloc((size_t)m * sizeof *out);
    int *cols = malloc((size_t)m * sizeof *cols);
    int next = 0, r, c, ok = 1;

    for (r = 0; r < m && ok; r++) {
        out[r] = malloc((size_t)n * sizeof **out);
        cols[r] = n;
        for (c = 0; c < n && ok; c++) {
            if (next == originalSize)
                ok = 0;
            else
                out[r][c] = original[next++];
        }
    }
    /* r is now the number of rows allocated. */
    if (ok && next != originalSize)
        ok = 0;
    if (!ok) {
        while (r > 0)
            free(out[--r]);
        free(out);
        free(cols);
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }
    *returnSize = m;
    *returnColumnSizes = cols;
    return out;
}
