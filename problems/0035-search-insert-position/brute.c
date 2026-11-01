/* Slow and obviously right, straight from the two cases of the statement:
 * look for target from the front; if it isn't there, it goes before the
 * first larger element, or at the end when there is none. */
int searchInsert(int *nums, int numsSize, int target)
{
    int i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] == target)
            return i;
    for (i = 0; i < numsSize; i++)
        if (nums[i] > target)
            return i;
    return numsSize;
}
