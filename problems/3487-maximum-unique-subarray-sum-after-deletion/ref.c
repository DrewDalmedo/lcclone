/* Reference solution: since any entry can be erased, the block can be any
 * entries with pairwise different values. If some value is positive, take
 * each distinct positive value once and nothing else; if none is, the best
 * block is the single largest entry. */
int maxSum(int *nums, int numsSize)
{
    bool seen[101] = {false};
    int sum = 0, largest = nums[0], i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] > largest)
            largest = nums[i];
        if (nums[i] > 0 && !seen[nums[i]]) {
            seen[nums[i]] = true;
            sum += nums[i];
        }
    }
    return largest > 0 ? sum : largest;
}
