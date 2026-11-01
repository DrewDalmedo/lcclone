/* Slow and obviously right: pair each element of nums1 with the first
 * element of nums2 that is equal to it and not paired yet; the paired
 * values are the answer. */
int *intersect(int *nums1, int nums1Size, int *nums2, int nums2Size,
               int *returnSize)
{
    int *out = malloc((size_t)nums1Size * sizeof *out);
    char *used = calloc((size_t)nums2Size, 1);
    int n = 0, i, k;

    for (i = 0; i < nums1Size; i++)
        for (k = 0; k < nums2Size; k++)
            if (!used[k] && nums2[k] == nums1[i]) {
                used[k] = 1;
                out[n++] = nums1[i];
                break;
            }
    free(used);
    *returnSize = n;
    return out;
}
