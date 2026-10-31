/* Reference solution: merge from the back. The largest value left in
 * either array goes to the last free slot of nums1, so a slot is always
 * written after the nums1 value it held (if any) has been read. Once
 * nums2 is used up, what is left of nums1 is already in place. */
void merge(int *nums1, int nums1Size, int m, int *nums2, int nums2Size, int n)
{
    int i = m - 1, j = n - 1, k = m + n - 1;

    (void)nums1Size;
    (void)nums2Size;
    while (j >= 0) {
        if (i >= 0 && nums1[i] > nums2[j])
            nums1[k--] = nums1[i--];
        else
            nums1[k--] = nums2[j--];
    }
}
