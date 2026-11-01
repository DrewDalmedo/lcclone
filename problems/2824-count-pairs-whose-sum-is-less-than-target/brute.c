/* Slow and obviously right: try every pair of positions i < j. */
int countPairs(int *nums, int numsSize, int target)
{
    int count = 0, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] + nums[j] < target)
                count++;
    return count;
}
