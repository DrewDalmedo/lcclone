/* Reference solution: count the 1s in every row and every column, then
 * count the 1s whose row and column each hold just that one. */
int numSpecial(int **mat, int matSize, int *matColSize)
{
    int m = matSize, n = matColSize[0], count = 0, i, j;
    int *rows = calloc((size_t)m, sizeof *rows);
    int *cols = calloc((size_t)n, sizeof *cols);

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (mat[i][j] == 1) {
                rows[i]++;
                cols[j]++;
            }
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (mat[i][j] == 1 && rows[i] == 1 && cols[j] == 1)
                count++;
    free(rows);
    free(cols);
    return count;
}
