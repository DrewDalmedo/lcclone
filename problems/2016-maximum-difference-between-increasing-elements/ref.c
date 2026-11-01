/* Reference solution: one pass, keeping the smallest number seen so far.
 * A number strictly above that minimum is a candidate gain; a number
 * below it becomes the new minimum for the numbers after it. */
int maximumDifference(int *nums, int numsSize)
{
    int low = nums[0], best = -1, i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] > low && nums[i] - low > best)
            best = nums[i] - low;
        if (nums[i] < low)
            low = nums[i];
    }
    return best;
}
