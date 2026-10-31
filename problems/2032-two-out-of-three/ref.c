/* Reference solution: give each value a 3-bit mask of the lists it is in,
 * then return the values whose mask has at least two bits set. */
static void mark(const int *nums, int size, int bit, int *mask)
{
    int i;

    for (i = 0; i < size; i++)
        mask[nums[i]] |= bit;
}

int *twoOutOfThree(int *nums1, int nums1Size, int *nums2, int nums2Size,
                   int *nums3, int nums3Size, int *returnSize)
{
    int mask[101] = {0}, v, m;
    int *out = malloc(100 * sizeof *out);

    mark(nums1, nums1Size, 1, mask);
    mark(nums2, nums2Size, 2, mask);
    mark(nums3, nums3Size, 4, mask);
    *returnSize = 0;
    for (v = 1; v <= 100; v++) {
        m = mask[v];
        if (m & (m - 1)) /* more than one bit set */
            out[(*returnSize)++] = v;
    }
    return out;
}
