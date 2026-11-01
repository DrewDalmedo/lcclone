/* Reference solution: copy the floor, then, unless the new colour is the
 * starting code already, walk the patch with an explicit stack. A tile is
 * repainted as it is pushed, so it no longer holds the starting code and is
 * never pushed twice: the stack never holds more than m * n tiles. */
int **floodFill(int **image, int imageSize, int *imageColSize, int sr, int sc,
                int color, int *returnSize, int **returnColumnSizes)
{
    static const int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    int m = imageSize, n = imageColSize[0], old = image[sr][sc];
    int i, j, k, top = 0, cell, r, c, nr, nc;
    int **out = malloc((size_t)m * sizeof *out);
    int *stack;

    *returnColumnSizes = malloc((size_t)m * sizeof **returnColumnSizes);
    for (i = 0; i < m; i++) {
        out[i] = malloc((size_t)n * sizeof **out);
        (*returnColumnSizes)[i] = n;
        for (j = 0; j < n; j++)
            out[i][j] = image[i][j];
    }
    *returnSize = m;
    if (color == old)
        return out;

    stack = malloc((size_t)m * n * sizeof *stack);
    out[sr][sc] = color;
    stack[top++] = sr * n + sc;
    while (top > 0) {
        cell = stack[--top];
        r = cell / n;
        c = cell % n;
        for (k = 0; k < 4; k++) {
            nr = r + dr[k];
            nc = c + dc[k];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && out[nr][nc] == old) {
                out[nr][nc] = color;
                stack[top++] = nr * n + nc;
            }
        }
    }
    free(stack);
    return out;
}
