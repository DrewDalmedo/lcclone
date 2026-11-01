/* Slow and obviously right: for each number from 1 to n * n, go over the
 * whole board and count the squares that hold it. */
int *findMissingAndRepeatedValues(int **grid, int gridSize,
                                  int *gridColSize, int *returnSize)
{
    int n = gridSize, v, i, j, count;
    int *out = malloc(2 * sizeof *out);

    out[0] = 0;
    out[1] = 0;
    for (v = 1; v <= n * n; v++) {
        count = 0;
        for (i = 0; i < n; i++)
            for (j = 0; j < gridColSize[i]; j++)
                if (grid[i][j] == v)
                    count++;
        if (count == 2)
            out[0] = v;
        if (count == 0)
            out[1] = v;
    }
    *returnSize = 2;
    return out;
}
