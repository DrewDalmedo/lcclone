/* Reference solution: two binary searches. The negatives end where the
 * first value >= 0 starts, and the positives start at the first value
 * >= 1; count each group from those two indices. */
static int first_at_least(const int *nums, int n, int value)
{
    int lo = 0, hi = n, mid;

    while (lo < hi) {
        mid = lo + (hi - lo) / 2;
        if (nums[mid] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

int maximumCount(int *nums, int numsSize)
{
    int neg = first_at_least(nums, numsSize, 0);
    int pos = numsSize - first_at_least(nums, numsSize, 1);

    return neg > pos ? neg : pos;
}
