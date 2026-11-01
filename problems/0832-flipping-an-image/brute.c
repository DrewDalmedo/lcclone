/* Slow and obviously right: do the two changes one after the other, each
 * into a grid of its own. First turn the slide over, then make the
 * negative of what that gives. */
int **flipAndInvertImage(int **image, int imageSize, int *imageColSize,
                         int *returnSize, int **returnColumnSizes)
{
    int n = imageSize, i, j;
    int *turned = malloc((size_t)n * n * sizeof *turned);
    int **out = malloc((size_t)n * sizeof *out);

    (void)imageColSize;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            turned[i * n + j] = image[i][n - 1 - j];
    *returnColumnSizes = malloc((size_t)n * sizeof **returnColumnSizes);
    for (i = 0; i < n; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++)
            out[i][j] = turned[i * n + j] == 1 ? 0 : 1;
    }
    *returnSize = n;
    free(turned);
    return out;
}
