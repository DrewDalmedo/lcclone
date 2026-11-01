/* Slow and obviously right: try every triple of indices i < j < k. */
int minimumSum(int *nums, int numsSize)
{
    int best = -1, i, j, k, sum;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            for (k = j + 1; k < numsSize; k++)
                if (nums[i] < nums[j] && nums[k] < nums[j]) {
                    sum = nums[i] + nums[j] + nums[k];
                    if (best == -1 || sum < best)
                        best = sum;
                }
    return best;
}
