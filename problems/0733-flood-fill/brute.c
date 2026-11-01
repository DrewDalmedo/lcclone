/* Slow and obviously right: the patch starts as the starting tile alone.
 * Sweep the whole floor again and again, adding every tile with the
 * starting code that has a neighbour already in the patch, until a sweep
 * adds nothing. Then paint the patch, reading codes from image itself. */
int **floodFill(int **image, int imageSize, int *imageColSize, int sr, int sc,
                int color, int *returnSize, int **returnColumnSizes)
{
    int m = imageSize, n = imageColSize[0], old = image[sr][sc];
    int i, j, grew;
    char *in = calloc((size_t)m * n, 1);
    int **out = malloc((size_t)m * sizeof *out);

    in[sr * n + sc] = 1;
    do {
        grew = 0;
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++) {
                if (in[i * n + j] || image[i][j] != old)
                    continue;
                if ((i > 0 && in[(i - 1) * n + j])
                    || (i + 1 < m && in[(i + 1) * n + j])
                    || (j > 0 && in[i * n + j - 1])
                    || (j + 1 < n && in[i * n + j + 1])) {
                    in[i * n + j] = 1;
                    grew = 1;
                }
            }
    } while (grew);

    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++)
            out[i][j] = in[i * n + j] ? color : image[i][j];
    }
    *returnSize = m;
    free(in);
    return out;
}
