/* Reference solution: build the rows from the top. Each row starts and
 * ends with 1, and each number in between adds the two numbers above it
 * in the row just built. Only sums are formed, and none exceeds the
 * largest number of row 29, C(29,14) = 77558760. */
int **generate(int numRows, int *returnSize, int **returnColumnSizes)
{
    int **rows = malloc((size_t)numRows * sizeof *rows);
    int i, j;

    *returnColumnSizes = malloc((size_t)numRows * sizeof **returnColumnSizes);
    for (i = 0; i < numRows; i++) {
        rows[i] = malloc((size_t)(i + 1) * sizeof **rows);
        (*returnColumnSizes)[i] = i + 1;
        rows[i][0] = rows[i][i] = 1;
        for (j = 1; j < i; j++)
            rows[i][j] = rows[i - 1][j - 1] + rows[i - 1][j];
    }
    *returnSize = numRows;
    return rows;
}
