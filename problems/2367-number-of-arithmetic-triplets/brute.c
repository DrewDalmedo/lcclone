/* Slow and obviously right: try every three positions i < j < k. */
int arithmeticTriplets(int *nums, int numsSize, int diff)
{
    int count = 0, i, j, k;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            for (k = j + 1; k < numsSize; k++)
                if (nums[j] - nums[i] == diff && nums[k] - nums[j] == diff)
                    count++;
    return count;
}
