/* Slow and obviously right: copy each row and run the k ticks on the
 * copy one at a time, moving every value by one column, then compare the
 * copy with the row. */
bool areSimilar(int **mat, int matSize, int *matColSize, int k)
{
    int n = matColSize[0], i, j, t, saved;
    int *row = malloc((size_t)n * sizeof *row);
    bool same = true;

    for (i = 0; i < matSize; i++) {
        for (j = 0; j < n; j++)
            row[j] = mat[i][j];
        for (t = 0; t < k; t++) {
            if (i % 2 == 0) {
                saved = row[0];
                for (j = 0; j < n - 1; j++)
                    row[j] = row[j + 1];
                row[n - 1] = saved;
            } else {
                saved = row[n - 1];
                for (j = n - 1; j > 0; j--)
                    row[j] = row[j - 1];
                row[0] = saved;
            }
        }
        for (j = 0; j < n; j++)
            if (row[j] != mat[i][j])
                same = false;
    }
    free(row);
    return same;
}
