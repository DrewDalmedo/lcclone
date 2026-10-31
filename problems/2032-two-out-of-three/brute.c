/* Slow and obviously right: for every value seen in any list, search each
 * list for it, count the lists that have it, and skip values already in
 * the answer. */
static int contains(const int *nums, int size, int v)
{
    int i;

    for (i = 0; i < size; i++)
        if (nums[i] == v)
            return 1;
    return 0;
}

int *twoOutOfThree(int *nums1, int nums1Size, int *nums2, int nums2Size,
                   int *nums3, int nums3Size, int *returnSize)
{
    int total = nums1Size + nums2Size + nums3Size, i, v, lists;
    int *all = malloc((size_t)total * sizeof *all);
    int *out = malloc((size_t)total * sizeof *out);

    for (i = 0; i < nums1Size; i++)
        all[i] = nums1[i];
    for (i = 0; i < nums2Size; i++)
        all[nums1Size + i] = nums2[i];
    for (i = 0; i < nums3Size; i++)
        all[nums1Size + nums2Size + i] = nums3[i];
    *returnSize = 0;
    for (i = 0; i < total; i++) {
        v = all[i];
        lists = contains(nums1, nums1Size, v) + contains(nums2, nums2Size, v) +
                contains(nums3, nums3Size, v);
        if (lists >= 2 && !contains(out, *returnSize, v))
            out[(*returnSize)++] = v;
    }
    free(all);
    return out;
}
