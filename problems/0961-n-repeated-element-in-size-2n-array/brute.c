/* Slow and obviously right: count how often each entry's value occurs in
 * the whole array, and return the one that occurs numsSize / 2 times. */
int repeatedNTimes(int *nums, int numsSize)
{
    int i, k, count;

    for (i = 0; i < numsSize; i++) {
        count = 0;
        for (k = 0; k < numsSize; k++)
            if (nums[k] == nums[i])
                count++;
        if (count == numsSize / 2)
            return nums[i];
    }
    return -1;
}
