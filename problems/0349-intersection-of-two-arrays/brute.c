/* Slow and obviously right: for each value of nums1, search nums2 for it
 * and the answer so far, and add it when it is in the first but not the
 * second. */
int *intersection(int *nums1, int nums1Size, int *nums2, int nums2Size,
                  int *returnSize)
{
    int *out = malloc((size_t)nums1Size * sizeof *out);
    int n = 0, i, k, found, listed;

    for (i = 0; i < nums1Size; i++) {
        found = 0;
        for (k = 0; k < nums2Size; k++)
            if (nums2[k] == nums1[i])
                found = 1;
        listed = 0;
        for (k = 0; k < n; k++)
            if (out[k] == nums1[i])
                listed = 1;
        if (found && !listed)
            out[n++] = nums1[i];
    }
    *returnSize = n;
    return out;
}
