/* Reference solution: read the value at the centre, then count the
 * entries that hold it; it is alone when the count is 1. */
bool isMiddleElementUnique(int *nums, int numsSize)
{
    int centre = nums[numsSize / 2], count = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] == centre)
            count++;
    return count == 1;
}
