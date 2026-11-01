/* Slow and obviously right: for each positive entry, look through the
 * whole array for its opposite, and keep the largest entry that has one. */
int findMaxK(int *nums, int numsSize)
{
    int best = -1, i, j;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] <= 0)
            continue;
        for (j = 0; j < numsSize; j++)
            if (nums[j] == -nums[i] && nums[i] > best)
                best = nums[i];
    }
    return best;
}
