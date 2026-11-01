/* Slow and obviously right: count the copies of each element, and return
 * the one that has a single copy. */
int singleNumber(int *nums, int numsSize)
{
    int i, j, count;

    for (i = 0; i < numsSize; i++) {
        count = 0;
        for (j = 0; j < numsSize; j++)
            if (nums[j] == nums[i])
                count++;
        if (count == 1)
            return nums[i];
    }
    return 0;
}
