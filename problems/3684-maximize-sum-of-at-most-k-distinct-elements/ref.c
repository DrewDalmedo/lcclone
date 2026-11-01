/* Reference solution: since every value is positive, the best selection
 * is the min(k, d) largest different values, d being how many different
 * values nums holds. Sort a copy of nums from largest to smallest and walk
 * it, keeping each value that differs from the last one kept, until k
 * values are kept or the copy runs out. */
static int larger_first(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

int *maxKDistinct(int *nums, int numsSize, int k, int *returnSize)
{
    int *sorted = malloc((size_t)numsSize * sizeof *sorted);
    int *out = malloc((size_t)k * sizeof *out);
    int m = 0, i;

    memcpy(sorted, nums, (size_t)numsSize * sizeof *sorted);
    qsort(sorted, (size_t)numsSize, sizeof *sorted, larger_first);
    for (i = 0; i < numsSize && m < k; i++)
        if (m == 0 || sorted[i] != out[m - 1])
            out[m++] = sorted[i];
    free(sorted);
    *returnSize = m;
    return out;
}
