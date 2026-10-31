/* Slow and obviously right: work out every number on its own, straight
 * from the definition, by recursion with no memory of earlier results: an
 * end of a row is 1, and any other number is the sum of the two above it.
 * That takes time exponential in the row, which is fine for the few rows
 * of the cross-check. */
static int entry(int i, int j)
{
    if (j == 0 || j == i)
        return 1;
    return entry(i - 1, j - 1) + entry(i - 1, j);
}

int **generate(int numRows, int *returnSize, int **returnColumnSizes)
{
    int **rows = malloc((size_t)numRows * sizeof *rows);
    int i, j;

    *returnColumnSizes = malloc((size_t)numRows * sizeof **returnColumnSizes);
    for (i = 0; i < numRows; i++) {
        rows[i] = malloc((size_t)(i + 1) * sizeof **rows);
        (*returnColumnSizes)[i] = i + 1;
        for (j = 0; j <= i; j++)
            rows[i][j] = entry(i, j);
    }
    *returnSize = numRows;
    return rows;
}
