/* Reference solution: one pass over the inner indices, adding each peak's
 * value and subtracting each valley's. */
int peakValleyDifference(int *nums, int numsSize)
{
    int total = 0, i;

    for (i = 1; i + 1 < numsSize; i++) {
        if (nums[i] > nums[i - 1] && nums[i] > nums[i + 1])
            total += nums[i];
        else if (nums[i] < nums[i - 1] && nums[i] < nums[i + 1])
            total -= nums[i];
    }
    return total;
}
