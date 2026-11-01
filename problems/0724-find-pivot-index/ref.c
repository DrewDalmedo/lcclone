/* Reference solution: add up the whole array first, then walk it from the
 * left keeping the sum of the left arm. The right arm is the total minus
 * the left arm minus the element on the pivot. Every sum stays within
 * 10^7 in absolute value, so int is wide enough. */
int pivotIndex(int *nums, int numsSize)
{
    int total = 0, left = 0, i;

    for (i = 0; i < numsSize; i++)
        total += nums[i];
    for (i = 0; i < numsSize; i++) {
        if (left == total - left - nums[i])
            return i;
        left += nums[i];
    }
    return -1;
}
