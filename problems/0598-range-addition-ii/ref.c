/* Reference solution: every rectangle covers the corner cell (0, 0), so
 * the largest number is ops.length, and the cells that hold it are the ones
 * every rectangle covers: the smallest a rows by the smallest b columns,
 * where the two smallest may come from different pairs. With no pairs,
 * that is the whole grid, m by n. */
int maxCount(int m, int n, int **ops, int opsSize, int *opsColSize)
{
    int rows = m, cols = n, i;

    (void)opsColSize;
    for (i = 0; i < opsSize; i++) {
        if (ops[i][0] < rows)
            rows = ops[i][0];
        if (ops[i][1] < cols)
            cols = ops[i][1];
    }
    return (int)((long long)rows * cols);
}
