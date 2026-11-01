/* Reference solution: one pass adding up the current rising slice. A
 * value that isn't bigger than the one before it starts a new slice with
 * itself, and the best total is updated after every value, so the last
 * slice counts too. */
int maxAscendingSum(int *nums, int numsSize)
{
    int sum = nums[0], best = nums[0], i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] > nums[i - 1])
            sum += nums[i];
        else
            sum = nums[i];
        if (sum > best)
            best = sum;
    }
    return best;
}
