/* Slow and obviously right: for each value of one array, search the other
 * array and the answer so far, and add it when it is in neither. */

static int contains(const int *a, int n, int x)
{
    int i;

    for (i = 0; i < n; i++)
        if (a[i] == x)
            return 1;
    return 0;
}

static int *only_in(const int *a, int na, const int *b, int nb, int *size)
{
    int *out = malloc((size_t)na * sizeof *out), i;

    *size = 0;
    for (i = 0; i < na; i++)
        if (!contains(b, nb, a[i]) && !contains(out, *size, a[i]))
            out[(*size)++] = a[i];
    return out;
}

int **findDifference(int *nums1, int nums1Size, int *nums2, int nums2Size,
                     int *returnSize, int **returnColumnSizes)
{
    int **out = malloc(2 * sizeof *out);

    *returnColumnSizes = malloc(2 * sizeof **returnColumnSizes);
    out[0] = only_in(nums1, nums1Size, nums2, nums2Size,
                     &(*returnColumnSizes)[0]);
    out[1] = only_in(nums2, nums2Size, nums1, nums1Size,
                     &(*returnColumnSizes)[1]);
    *returnSize = 2;
    return out;
}
