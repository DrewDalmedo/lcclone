/* Reference solution: sort the scores. Some best group is always k
 * neighbours in sorted order (swapping a member for a score between the
 * group's lowest and highest never widens the gap), so take the smallest
 * sorted[i + k - 1] - sorted[i]. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int minimumDifference(int *nums, int numsSize, int k)
{
    int best, i;

    qsort(nums, (size_t)numsSize, sizeof *nums, cmp_int);
    best = nums[k - 1] - nums[0];
    for (i = 1; i + k <= numsSize; i++)
        if (nums[i + k - 1] - nums[i] < best)
            best = nums[i + k - 1] - nums[i];
    return best;
}
