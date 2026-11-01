/* Reference solution: count how many squares hold each number in a table
 * of n * n + 1 entries (numbers go up to n * n). The number counted twice
 * and the number counted zero times are the answer. */
int *findMissingAndRepeatedValues(int **grid, int gridSize,
                                  int *gridColSize, int *returnSize)
{
    int n = gridSize, total = n * n, i, j, v;
    int *count = calloc((size_t)total + 1, sizeof *count);
    int *out = malloc(2 * sizeof *out);

    (void)gridColSize;
    out[0] = 0;
    out[1] = 0;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            count[grid[i][j]]++;
    for (v = 1; v <= total; v++) {
        if (count[v] == 2)
            out[0] = v;
        else if (count[v] == 0)
            out[1] = v;
    }
    free(count);
    *returnSize = 2;
    return out;
}
