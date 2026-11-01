/* Slow and obviously right: try every earlier and later position, keeping
 * the pairs where the later number is strictly larger. */
int maximumDifference(int *nums, int numsSize)
{
    int best = -1, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] < nums[j] && nums[j] - nums[i] > best)
                best = nums[j] - nums[i];
    return best;
}
