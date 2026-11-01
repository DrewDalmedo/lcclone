/* Reference solution: for each summit j, the best feet are the smallest
 * element on each side of it, if both are below nums[j]. Precompute the
 * smallest element to the right of each index, then sweep from the left
 * keeping the smallest element seen so far. */
int minimumSum(int *nums, int numsSize)
{
    int right[50], best = INT_MAX, left, j;

    right[numsSize - 1] = INT_MAX;
    for (j = numsSize - 2; j >= 0; j--)
        right[j] = nums[j + 1] < right[j + 1] ? nums[j + 1] : right[j + 1];
    left = nums[0];
    for (j = 1; j < numsSize - 1; j++) {
        if (left < nums[j] && right[j] < nums[j]
            && left + nums[j] + right[j] < best)
            best = left + nums[j] + right[j];
        if (nums[j] < left)
            left = nums[j];
    }
    return best == INT_MAX ? -1 : best;
}
