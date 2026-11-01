/* Reference solution: for each pixel, add up the rows i - 1 to i + 1 and
 * the columns j - 1 to j + 1 around it, clipped to the picture, and divide
 * by the number of pixels added (integer division rounds down, as nothing
 * is negative). The sums are read from img and written to a new grid, so
 * no pixel sees an already blurred neighbour. */
int **imageSmoother(int **img, int imgSize, int *imgColSize, int *returnSize,
                    int **returnColumnSizes)
{
    int m = imgSize, n = imgColSize[0], i, j, x, y, x0, x1, y0, y1, sum;
    int **out = malloc((size_t)m * sizeof *out);

    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        x0 = i > 0 ? i - 1 : 0;
        x1 = i < m - 1 ? i + 1 : m - 1;
        for (j = 0; j < n; j++) {
            y0 = j > 0 ? j - 1 : 0;
            y1 = j < n - 1 ? j + 1 : n - 1;
            sum = 0;
            for (x = x0; x <= x1; x++)
                for (y = y0; y <= y1; y++)
                    sum += img[x][y];
            out[i][j] = sum / ((x1 - x0 + 1) * (y1 - y0 + 1));
        }
    }
    *returnSize = m;
    return out;
}
