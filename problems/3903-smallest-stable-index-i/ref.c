/* Reference solution: fill in the smallest value of every suffix from
 * the right, then walk from the left keeping the largest value so far,
 * and stop at the first position whose two values are at most k apart. */
int firstStableIndex(int *nums, int numsSize, int k)
{
    int *low = malloc((size_t)numsSize * sizeof *low);
    int high, i, ans = -1;

    low[numsSize - 1] = nums[numsSize - 1];
    for (i = numsSize - 2; i >= 0; i--)
        low[i] = nums[i] < low[i + 1] ? nums[i] : low[i + 1];
    high = nums[0];
    for (i = 0; i < numsSize; i++) {
        if (nums[i] > high)
            high = nums[i];
        if (high - low[i] <= k) {
            ans = i;
            break;
        }
    }
    free(low);
    return ans;
}
