/* Slow and obviously right: try every pair of indices i < j and test both
 * conditions as stated. */
int countPairs(int *nums, int numsSize, int k)
{
    int count = 0, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] == nums[j] && (i * j) % k == 0)
                count++;
    return count;
}
