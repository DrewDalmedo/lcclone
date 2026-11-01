/* Reference solution: sort a copy of arr2. For each x of arr1, binary
 * search for the first entry of arr2 that is at least x - d; x keeps
 * clear exactly when there is none, or it is greater than x + d. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int findTheDistanceValue(int *arr1, int arr1Size, int *arr2, int arr2Size,
                         int d)
{
    int *sorted = malloc((size_t)arr2Size * sizeof *sorted);
    int count = 0, i, lo, hi, mid, x;

    memcpy(sorted, arr2, (size_t)arr2Size * sizeof *sorted);
    qsort(sorted, (size_t)arr2Size, sizeof *sorted, cmp_int);
    for (i = 0; i < arr1Size; i++) {
        x = arr1[i];
        lo = 0;
        hi = arr2Size;                  /* first index with value >= x - d */
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (sorted[mid] < x - d)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo == arr2Size || sorted[lo] > x + d)
            count++;
    }
    free(sorted);
    return count;
}
