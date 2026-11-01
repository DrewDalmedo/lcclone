/* Slow and obviously right: test every pair of indices i < j. */
int countKDifference(int *nums, int numsSize, int k)
{
    int count = 0, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (abs(nums[i] - nums[j]) == k)
                count++;
    return count;
}
