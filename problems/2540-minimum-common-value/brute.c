/* Slow and obviously right: compare every timestamp of nums1 with every
 * timestamp of nums2, and keep the smallest one that matches, without
 * relying on the order of either log. */
int getCommon(int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    int best = -1, i, j;

    for (i = 0; i < nums1Size; i++)
        for (j = 0; j < nums2Size; j++)
            if (nums1[i] == nums2[j] && (best == -1 || nums1[i] < best))
                best = nums1[i];
    return best;
}
