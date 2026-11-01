/* Slow and obviously right: for each entry, count the entries equal to it
 * by scanning the whole array, and add it when that count is 1. */
int sumOfUnique(int *nums, int numsSize)
{
    int sum = 0, i, j, same;

    for (i = 0; i < numsSize; i++) {
        same = 0;
        for (j = 0; j < numsSize; j++)
            if (nums[j] == nums[i])
                same++;
        if (same == 1)
            sum += nums[i];
    }
    return sum;
}
