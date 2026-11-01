/* Slow and obviously right: try every slice nums[l..r], check that it
 * rises, and add it up from scratch. */
int maxAscendingSum(int *nums, int numsSize)
{
    int best = 0, sum, rising, l, r, i;

    for (l = 0; l < numsSize; l++)
        for (r = l; r < numsSize; r++) {
            rising = 1;
            sum = 0;
            for (i = l; i <= r; i++) {
                if (i > l && nums[i] <= nums[i - 1])
                    rising = 0;
                sum += nums[i];
            }
            if (rising && sum > best)
                best = sum;
        }
    return best;
}
