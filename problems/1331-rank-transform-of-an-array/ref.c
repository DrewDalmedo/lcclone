/* Reference solution: sort a copy of arr and drop its repeats, which
 * leaves the different values in order; the rank of an element is then
 * 1 plus its position there, found by binary search. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int *arrayRankTransform(int *arr, int arrSize, int *returnSize)
{
    int *vals, *out, d = 0, i, lo, hi, mid;

    *returnSize = arrSize;
    if (arrSize == 0)
        return malloc(sizeof(int)); /* an empty result */
    vals = malloc((size_t)arrSize * sizeof *vals);
    out = malloc((size_t)arrSize * sizeof *out);
    memcpy(vals, arr, (size_t)arrSize * sizeof *vals);
    qsort(vals, (size_t)arrSize, sizeof *vals, cmp_int);
    for (i = 0; i < arrSize; i++)
        if (d == 0 || vals[i] != vals[d - 1])
            vals[d++] = vals[i];
    for (i = 0; i < arrSize; i++) {
        lo = 0;
        hi = d - 1;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (vals[mid] < arr[i])
                lo = mid + 1;
            else
                hi = mid;
        }
        out[i] = lo + 1;
    }
    free(vals);
    return out;
}
