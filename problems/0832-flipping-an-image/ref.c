/* Reference solution: fill each row of the result from both ends at once.
 * The dots at l and r = n - 1 - l trade places and are both flipped, so
 * each gets the opposite of the other's value; when l and r meet in the
 * middle of an odd row, that one dot is just flipped. */
int **flipAndInvertImage(int **image, int imageSize, int *imageColSize,
                         int *returnSize, int **returnColumnSizes)
{
    int n = imageSize, i, l, r, a, b;
    int **out = malloc((size_t)n * sizeof *out);

    (void)imageColSize;
    *returnColumnSizes = malloc((size_t)n * sizeof **returnColumnSizes);
    for (i = 0; i < n; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (l = 0, r = n - 1; l <= r; l++, r--) {
            a = image[i][l];
            b = image[i][r];
            out[i][l] = 1 - b;
            out[i][r] = 1 - a;
        }
    }
    *returnSize = n;
    return out;
}
