/* Reference solution: measure every number by counting its decimal digits
 * (zero has one), adding one for a minus sign, and keep the largest length
 * of each column. */
static int text_length(int x)
{
    long long v = x;
    int len = x <= 0 ? 1 : 0; /* the '-', or the digit of 0 */

    if (v < 0)
        v = -v;
    while (v > 0) {
        len++;
        v /= 10;
    }
    return len;
}

int *findColumnWidth(int **grid, int gridSize, int *gridColSize,
                     int *returnSize)
{
    int m = gridSize, n = gridColSize[0], r, c, len;
    int *width = malloc((size_t)n * sizeof *width);

    for (c = 0; c < n; c++) {
        width[c] = 0;
        for (r = 0; r < m; r++) {
            len = text_length(grid[r][c]);
            if (len > width[c])
                width[c] = len;
        }
    }
    *returnSize = n;
    return width;
}
