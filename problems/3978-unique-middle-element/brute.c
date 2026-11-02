/* Slow and obviously right: find the centre as the position with as many
 * entries before it as after it, then compare its value with that of
 * every other position. */
bool isMiddleElementUnique(int *nums, int numsSize)
{
    int c = 0, i, j;

    for (i = 0; i < numsSize; i++)
        if (i == numsSize - 1 - i)
            c = i;
    for (j = 0; j < numsSize; j++)
        if (j != c && nums[j] == nums[c])
            return false;
    return true;
}
