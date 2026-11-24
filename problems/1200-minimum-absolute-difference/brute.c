/* Slow and obviously right: mark every number in a table over
 * -10^6..10^6. Then try each distance d = 1, 2, ... in turn, and stop at
 * the first one that some number v has to another (v + d marked too):
 * that is the smallest distance. Walking the table upward then lists every
 * pair [v, v + d] in order of its first number. Trying a distance looks
 * at every number, and the smallest distance is at most the range over
 * n - 1, so this takes about as many steps as the range has values. */
int **minimumAbsDifference(int *arr, int arrSize, int *returnSize,
                           int **returnColumnSizes)
{
    char *mark = calloc(2000001, 1);
    int **out = malloc((size_t)arrSize * sizeof *out);
    int d, found = 0, n = 0, i, v;

    for (i = 0; i < arrSize; i++)
        mark[arr[i] + 1000000] = 1;
    for (d = 1; !found; d++)
        for (i = 0; i < arrSize && !found; i++)
            if (arr[i] + d <= 1000000 && mark[arr[i] + d + 1000000])
                found = 1;
    d--;
    *returnColumnSizes = malloc((size_t)arrSize * sizeof **returnColumnSizes);
    for (v = -1000000; v + d <= 1000000; v++)
        if (mark[v + 1000000] && mark[v + d + 1000000]) {
            out[n] = malloc(2 * sizeof **out);
            out[n][0] = v;
            out[n][1] = v + d;
            (*returnColumnSizes)[n] = 2;
            n++;
        }
    free(mark);
    *returnSize = n;
    return out;
}
