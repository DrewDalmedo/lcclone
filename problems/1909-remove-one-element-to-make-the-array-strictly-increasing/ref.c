/* Reference solution: find the first element that is not larger than the
 * one before it. If there is none, nums is already strictly increasing.
 * Otherwise taking out any other element leaves those two side by side, so
 * only two choices can work: check nums without the first of the two and
 * without the second. */
static bool increasing_without(const int *nums, int n, int skip)
{
    int prev = 0, i; /* every value is at least 1 */

    for (i = 0; i < n; i++) {
        if (i == skip)
            continue;
        if (nums[i] <= prev)
            return false;
        prev = nums[i];
    }
    return true;
}

bool canBeIncreasing(int *nums, int numsSize)
{
    int i;

    for (i = 1; i < numsSize; i++)
        if (nums[i - 1] >= nums[i])
            return increasing_without(nums, numsSize, i - 1) ||
                   increasing_without(nums, numsSize, i);
    return true;
}
