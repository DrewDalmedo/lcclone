/* Reference solution: adding x moves the smallest element to the smallest
 * element, whatever the order, so x is min(nums2) - min(nums1). */
int addedInteger(int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    int min1 = nums1[0], min2 = nums2[0], i;

    for (i = 1; i < nums1Size; i++)
        if (nums1[i] < min1)
            min1 = nums1[i];
    for (i = 1; i < nums2Size; i++)
        if (nums2[i] < min2)
            min2 = nums2[i];
    return min2 - min1;
}
