/* Slow and obviously right: walk the island from its first land cell (a
 * depth-first search with an explicit stack), and for every cell reached
 * count the sides whose neighbour is water or off the map. */
int islandPerimeter(int **grid, int gridSize, int *gridColSize)
{
    static const int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    int rows = gridSize, cols = gridColSize[0], top = 0, count = 0;
    int i, j, k, cell, r, c, nr, nc;
    char *seen = calloc((size_t)rows * cols, 1);
    int *stack = malloc((size_t)rows * cols * sizeof *stack);

    for (i = 0; i < rows && top == 0; i++)
        for (j = 0; j < cols && top == 0; j++)
            if (grid[i][j] == 1) {
                seen[i * cols + j] = 1;
                stack[top++] = i * cols + j;
            }
    while (top > 0) {
        cell = stack[--top];
        r = cell / cols;
        c = cell % cols;
        for (k = 0; k < 4; k++) {
            nr = r + dr[k];
            nc = c + dc[k];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols
                || grid[nr][nc] == 0)
                count++;
            else if (!seen[nr * cols + nc]) {
                seen[nr * cols + nc] = 1;
                stack[top++] = nr * cols + nc;
            }
        }
    }
    free(seen);
    free(stack);
    return count;
}
