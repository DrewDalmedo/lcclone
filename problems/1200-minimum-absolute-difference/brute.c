/* Slow and obviously right: try every two numbers to find the smallest
 * distance d, then collect every pair d apart as [smaller, larger] and
 * put the pairs in order of their first number by insertion sort. */
int **minimumAbsDifference(int *arr, int arrSize, int *returnSize,
                           int **returnColumnSizes)
{
    int **out = malloc((size_t)arrSize * sizeof *out);
    int d = -1, diff, n = 0, i, j;
    int *t;

    for (i = 0; i < arrSize; i++)
        for (j = i + 1; j < arrSize; j++) {
            diff = abs(arr[i] - arr[j]);
            if (d < 0 || diff < d)
                d = diff;
        }
    for (i = 0; i < arrSize; i++)
        for (j = 0; j < arrSize; j++)
            if (arr[j] - arr[i] == d) {
                out[n] = malloc(2 * sizeof **out);
                out[n][0] = arr[i];
                out[n][1] = arr[j];
                n++;
            }
    for (i = 1; i < n; i++)
        for (j = i; j > 0 && out[j - 1][0] > out[j][0]; j--) {
            t = out[j];
            out[j] = out[j - 1];
            out[j - 1] = t;
        }
    *returnColumnSizes = malloc((size_t)arrSize * sizeof **returnColumnSizes);
    for (i = 0; i < n; i++)
        (*returnColumnSizes)[i] = 2;
    *returnSize = n;
    return out;
}
