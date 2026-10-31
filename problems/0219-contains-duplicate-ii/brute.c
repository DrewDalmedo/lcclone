/* Slow and obviously right: compare every position with each of the k
 * positions after it. */
bool containsNearbyDuplicate(int *nums, int numsSize, int k)
{
    int i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize && j - i <= k; j++)
            if (nums[i] == nums[j])
                return true;
    return false;
}
