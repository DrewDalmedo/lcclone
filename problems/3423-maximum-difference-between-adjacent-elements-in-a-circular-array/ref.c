/* Reference solution: one pass over the n stretches, the last one
 * wrapping from nums[n - 1] back to nums[0]. */
int maxAdjacentDistance(int *nums, int numsSize)
{
    int best = 0, i, d;

    for (i = 0; i < numsSize; i++) {
        d = nums[i] - nums[(i + 1) % numsSize];
        if (d < 0)
            d = -d;
        if (d > best)
            best = d;
    }
    return best;
}
