/* Slow and obviously right: try every pair (i, j) and test the remainder. */
int numberOfPairs(int *nums1, int nums1Size, int *nums2, int nums2Size, int k)
{
    int total = 0, i, j;

    for (i = 0; i < nums1Size; i++)
        for (j = 0; j < nums2Size; j++)
            if (nums1[i] % (nums2[j] * k) == 0)
                total++;
    return total;
}
