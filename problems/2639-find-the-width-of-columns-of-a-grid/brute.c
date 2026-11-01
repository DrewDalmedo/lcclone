/* Slow and obviously right: print every number with snprintf and take the
 * length of the text, keeping the longest of each column. */
int *findColumnWidth(int **grid, int gridSize, int *gridColSize,
                     int *returnSize)
{
    int n = gridColSize[0], r, c, len;
    int *width = malloc((size_t)n * sizeof *width);
    char text[32];

    for (c = 0; c < n; c++) {
        width[c] = 0;
        for (r = 0; r < gridSize; r++) {
            len = snprintf(text, sizeof text, "%d", grid[r][c]);
            if (len > width[c])
                width[c] = len;
        }
    }
    *returnSize = n;
    return width;
}
