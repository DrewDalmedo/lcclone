/* Reference solution, by cases on k.
 * - k == n: one window covers everything, so every value counts once and
 *   the answer is the largest element.
 * - k == 1: each window is one element, so a value's count is its number
 *   of copies; the answer is the largest value with exactly one copy.
 * - otherwise every element except the first and the last lies in two or
 *   more windows. nums[0] is only in the first window and nums[n - 1] only
 *   in the last, so each counts once exactly when it has no other copy
 *   anywhere in nums. */
static int copies(const int *nums, int n, int v)
{
    int c = 0, i;

    for (i = 0; i < n; i++)
        if (nums[i] == v)
            c++;
    return c;
}

int largestInteger(int *nums, int numsSize, int k)
{
    int n = numsSize, best = -1, i;

    if (k == n) {
        for (i = 0; i < n; i++)
            if (nums[i] > best)
                best = nums[i];
    } else if (k == 1) {
        for (i = 0; i < n; i++)
            if (nums[i] > best && copies(nums, n, nums[i]) == 1)
                best = nums[i];
    } else {
        if (copies(nums, n, nums[0]) == 1)
            best = nums[0];
        if (nums[n - 1] > best && copies(nums, n, nums[n - 1]) == 1)
            best = nums[n - 1];
    }
    return best;
}
