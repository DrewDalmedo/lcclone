/* Slow and obviously right: build the whole triangle down to the row
 * asked for, every row in full, each number in between the 1s the sum of
 * the two above it, and copy out the last row. */
int *getRow(int rowIndex, int *returnSize)
{
    int t[34][34];
    int *row = malloc((size_t)(rowIndex + 1) * sizeof *row);
    int i, j;

    for (i = 0; i <= rowIndex; i++) {
        t[i][0] = t[i][i] = 1;
        for (j = 1; j < i; j++)
            t[i][j] = t[i - 1][j - 1] + t[i - 1][j];
    }
    for (j = 0; j <= rowIndex; j++)
        row[j] = t[rowIndex][j];
    *returnSize = rowIndex + 1;
    return row;
}
