/* Reference solution: sort, then each round removes the next element from
 * the front and the next from the back, so the averages are those of
 * a[i] and a[n-1-i]; keep the smallest pair sum and halve it at the end. */
static int cmp_int(const void *x, const void *y)
{
    int a = *(const int *)x, b = *(const int *)y;

    return (a > b) - (a < b);
}

double minimumAverage(int *nums, int numsSize)
{
    int *a = malloc((size_t)numsSize * sizeof *a);
    int i, best, sum;

    memcpy(a, nums, (size_t)numsSize * sizeof *a);
    qsort(a, (size_t)numsSize, sizeof *a, cmp_int);
    best = a[0] + a[numsSize - 1];
    for (i = 1; i < numsSize / 2; i++) {
        sum = a[i] + a[numsSize - 1 - i];
        if (sum < best)
            best = sum;
    }
    free(a);
    return best / 2.0;
}
