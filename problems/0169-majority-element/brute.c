/* Slow and obviously right: count the copies of each element, and return
 * the first that fills more than half of the positions. */
int majorityElement(int *nums, int numsSize)
{
    int i, j, count;

    for (i = 0; i < numsSize; i++) {
        count = 0;
        for (j = 0; j < numsSize; j++)
            if (nums[j] == nums[i])
                count++;
        if (count > numsSize / 2)
            return nums[i];
    }
    return 0;
}
