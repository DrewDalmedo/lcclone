/* Slow and obviously right: copy out each column, sort the copy, and
 * count the columns that sorting changed. */
int minDeletionSize(char **strs, int strsSize)
{
    int m = (int)strlen(strs[0]), count = 0, i, j, k;
    char *col = malloc((size_t)strsSize), *sorted = malloc((size_t)strsSize);
    char t;

    for (j = 0; j < m; j++) {
        for (i = 0; i < strsSize; i++)
            col[i] = sorted[i] = strs[i][j];
        for (i = 1; i < strsSize; i++)
            for (k = i; k > 0 && sorted[k - 1] > sorted[k]; k--) {
                t = sorted[k];
                sorted[k] = sorted[k - 1];
                sorted[k - 1] = t;
            }
        if (memcmp(col, sorted, (size_t)strsSize) != 0)
            count++;
    }
    free(col);
    free(sorted);
    return count;
}
