/* Slow and obviously right: try every pair of positions, and keep the
 * distance of those that are neighbours on the loop, in either order. */
int maxAdjacentDistance(int *nums, int numsSize)
{
    int n = numsSize, best = -1, i, j, d;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (i == j || (j != (i + 1) % n && i != (j + 1) % n))
                continue;
            d = nums[i] > nums[j] ? nums[i] - nums[j] : nums[j] - nums[i];
            if (d > best)
                best = d;
        }
    return best;
}
