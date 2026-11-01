/* Slow and obviously right: for each pixel, look at every pixel of the
 * picture and add in the ones at most one row and one column away (the
 * pixel itself included), counting them; then divide, rounding down. */
int **imageSmoother(int **img, int imgSize, int *imgColSize, int *returnSize,
                    int **returnColumnSizes)
{
    int **out = malloc((size_t)imgSize * sizeof *out);
    int i, j, x, y, sum, count;

    *returnColumnSizes = malloc((size_t)imgSize * sizeof **returnColumnSizes);
    for (i = 0; i < imgSize; i++) {
        out[i] = malloc((size_t)imgColSize[i] * sizeof **out);
        (*returnColumnSizes)[i] = imgColSize[i];
        for (j = 0; j < imgColSize[i]; j++) {
            sum = 0;
            count = 0;
            for (x = 0; x < imgSize; x++)
                for (y = 0; y < imgColSize[x]; y++)
                    if (x - i >= -1 && x - i <= 1 && y - j >= -1
                        && y - j <= 1) {
                        sum += img[x][y];
                        count++;
                    }
            out[i][j] = (int)floor((double)sum / count);
        }
    }
    *returnSize = imgSize;
    return out;
}
