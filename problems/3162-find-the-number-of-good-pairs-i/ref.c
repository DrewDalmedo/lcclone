/* Reference solution: count how often each value occurs in nums1 (all
 * are at most 50). For each j, the values of nums1 that are multiples of
 * step = nums2[j] * k are step, 2 * step, ... up to 50, so add up their
 * counts; a step above 50 adds nothing. */
int numberOfPairs(int *nums1, int nums1Size, int *nums2, int nums2Size, int k)
{
    int count[51] = {0};
    int total = 0, i, j, step, v;

    for (i = 0; i < nums1Size; i++)
        count[nums1[i]]++;
    for (j = 0; j < nums2Size; j++) {
        step = nums2[j] * k;
        for (v = step; v <= 50; v += step)
            total += count[v];
    }
    return total;
}
