/* Slow and obviously right: copy the m values of nums1 and the n values
 * of nums2 into a new array, sort it with insertion sort, and copy it
 * back over nums1. */
void merge(int *nums1, int nums1Size, int m, int *nums2, int nums2Size, int n)
{
    int all[200];
    int i, j, v, total = 0;

    (void)nums1Size;
    (void)nums2Size;
    for (i = 0; i < m; i++)
        all[total++] = nums1[i];
    for (i = 0; i < n; i++)
        all[total++] = nums2[i];
    for (i = 1; i < total; i++) {
        v = all[i];
        for (j = i; j > 0 && all[j - 1] > v; j--)
            all[j] = all[j - 1];
        all[j] = v;
    }
    for (i = 0; i < total; i++)
        nums1[i] = all[i];
}
