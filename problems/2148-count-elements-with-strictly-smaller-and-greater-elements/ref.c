/* Reference solution: an element has something below it and something
 * above it exactly when it lies strictly between the minimum and the
 * maximum. Find both in one pass, then count in a second. */
int countElements(int *nums, int numsSize)
{
    int lo = nums[0], hi = nums[0], count = 0, i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] < lo)
            lo = nums[i];
        if (nums[i] > hi)
            hi = nums[i];
    }
    for (i = 0; i < numsSize; i++)
        if (lo < nums[i] && nums[i] < hi)
            count++;
    return count;
}
