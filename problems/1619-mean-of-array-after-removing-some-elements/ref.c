/* Reference solution: sort, then average the elements between the first
 * n / 20 and the last n / 20. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

double trimMean(int *arr, int arrSize)
{
    int t = arrSize / 20, i;
    int *s = malloc((size_t)arrSize * sizeof *s);
    long long sum = 0;

    memcpy(s, arr, (size_t)arrSize * sizeof *s);
    qsort(s, (size_t)arrSize, sizeof *s, cmp_int);
    for (i = t; i < arrSize - t; i++)
        sum += s[i];
    free(s);
    return (double)sum / (arrSize - 2 * t);
}
