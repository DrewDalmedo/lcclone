/* Reference solution: walk both sorted logs with one index each, always
 * stepping past the smaller value; the first equal pair met is the
 * earliest common timestamp. O(n + m) time. */
int getCommon(int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    int i = 0, j = 0;

    while (i < nums1Size && j < nums2Size) {
        if (nums1[i] == nums2[j])
            return nums1[i];
        if (nums1[i] < nums2[j])
            i++;
        else
            j++;
    }
    return -1;
}
