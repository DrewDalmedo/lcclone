/* Slow and obviously right: look at every pair of positions i < j. */
int numIdenticalPairs(int *nums, int numsSize)
{
    int pairs = 0, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] == nums[j])
                pairs++;
    return pairs;
}
