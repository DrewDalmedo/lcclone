/* Reference solution: sort a copy of the numbers. The closest two are
 * then neighbours, so one pass finds the smallest gap between neighbours
 * and a second collects, left to right, the neighbours that gap apart. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int **minimumAbsDifference(int *arr, int arrSize, int *returnSize,
                           int **returnColumnSizes)
{
    int *s = malloc((size_t)arrSize * sizeof *s);
    int **out;
    int best, count = 0, i, k = 0;

    memcpy(s, arr, (size_t)arrSize * sizeof *s);
    qsort(s, (size_t)arrSize, sizeof *s, cmp_int);
    best = s[1] - s[0];
    for (i = 2; i < arrSize; i++)
        if (s[i] - s[i - 1] < best)
            best = s[i] - s[i - 1];
    for (i = 1; i < arrSize; i++)
        if (s[i] - s[i - 1] == best)
            count++;
    out = malloc((size_t)count * sizeof *out);
    *returnColumnSizes = malloc((size_t)count * sizeof **returnColumnSizes);
    for (i = 1; i < arrSize; i++)
        if (s[i] - s[i - 1] == best) {
            out[k] = malloc(2 * sizeof **out);
            out[k][0] = s[i - 1];
            out[k][1] = s[i];
            (*returnColumnSizes)[k] = 2;
            k++;
        }
    free(s);
    *returnSize = count;
    return out;
}
