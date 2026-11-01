/* Reference solution: look at d = 0, 1, 2, ... places to the left and to
 * the right of start, and stop at the first d where either side holds
 * target. */
int getMinDistance(int *nums, int numsSize, int target, int start)
{
    int d;

    for (d = 0; d < numsSize; d++) {
        if (start - d >= 0 && nums[start - d] == target)
            return d;
        if (start + d < numsSize && nums[start + d] == target)
            return d;
    }
    return -1; /* not reached: target occurs in nums */
}
