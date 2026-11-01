/* Reference solution: the walk keeps a cell exactly when its row plus
 * its column is even, since each step of the walk changes r + c by 1
 * (along a row, or straight down at a row's end). So take the even
 * columns of even rows left to right, and the odd columns of odd rows
 * right to left. */
int *zigzagTraversal(int **grid, int gridSize, int *gridColSize,
                     int *returnSize)
{
    int rows = gridSize, cols = gridColSize[0], r, c, k = 0;
    int *out = malloc((size_t)(rows * cols + 1) / 2 * sizeof *out);

    for (r = 0; r < rows; r++) {
        if (r % 2 == 0)
            for (c = 0; c < cols; c += 2)
                out[k++] = grid[r][c];
        else
            for (c = cols % 2 == 0 ? cols - 1 : cols - 2; c >= 1; c -= 2)
                out[k++] = grid[r][c];
    }
    *returnSize = k;
    return out;
}
