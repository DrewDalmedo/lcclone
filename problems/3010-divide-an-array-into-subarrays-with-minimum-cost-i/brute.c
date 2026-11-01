/* Slow and obviously right: try every pair of cuts. The second piece
 * starts at i and the third at j, with 1 <= i < j <= n - 1, so that no
 * piece is empty; the three prices are nums[0], nums[i] and nums[j]. */
int minimumCost(int *nums, int numsSize)
{
    int best = INT_MAX, i, j, cost;

    for (i = 1; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++) {
            cost = nums[0] + nums[i] + nums[j];
            if (cost < best)
                best = cost;
        }
    return best;
}
