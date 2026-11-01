/* Reference solution: mark the digits of each list in a bit mask. If the
 * masks share a digit, the smallest shared one is the answer on its own;
 * otherwise write the smaller of the two minimums, then the larger. */
int minNumber(int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    int mask1 = 0, mask2 = 0, a = 10, b = 10, d, i;

    for (i = 0; i < nums1Size; i++) {
        mask1 |= 1 << nums1[i];
        if (nums1[i] < a)
            a = nums1[i];
    }
    for (i = 0; i < nums2Size; i++) {
        mask2 |= 1 << nums2[i];
        if (nums2[i] < b)
            b = nums2[i];
    }
    for (d = 1; d <= 9; d++)
        if ((mask1 & mask2) & (1 << d))
            return d;
    return a < b ? 10 * a + b : 10 * b + a;
}
