/* Slow and obviously right: compare the neighbour sums at every two
 * different indices i < j, adding in long long. */
bool findSubarrays(int *nums, int numsSize)
{
    int i, j;

    for (i = 0; i + 1 < numsSize; i++)
        for (j = i + 1; j + 1 < numsSize; j++)
            if ((long long)nums[i] + nums[i + 1]
                == (long long)nums[j] + nums[j + 1])
                return true;
    return false;
}
