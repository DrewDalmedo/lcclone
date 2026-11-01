/* Reference solution: add up the whole array once, then walk it from the
 * left, keeping the sum of the piles already passed. At pile i the right
 * side holds total - left - nums[i]. The total is at most 10^8, so int is
 * wide enough. */
int *leftRightDifference(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int total = 0, left = 0, right, i;

    for (i = 0; i < numsSize; i++)
        total += nums[i];
    for (i = 0; i < numsSize; i++) {
        right = total - left - nums[i];
        out[i] = left > right ? left - right : right - left;
        left += nums[i];
    }
    *returnSize = numsSize;
    return out;
}
