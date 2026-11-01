/* Reference solution: one pass over the middle positions, comparing each
 * value with twice the sum of its neighbours, so that nothing odd is ever
 * halved. */
int countSubarrays(int *nums, int numsSize)
{
    int count = 0, j;

    for (j = 1; j + 1 < numsSize; j++)
        if (nums[j] == 2 * (nums[j - 1] + nums[j + 1]))
            count++;
    return count;
}
