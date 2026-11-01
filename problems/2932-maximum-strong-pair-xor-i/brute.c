/* Slow and obviously right: try every ordered pair of positions, the same
 * one twice included, and keep the largest XOR among the strong pairs,
 * tested in another form: the two values differ by at most the smaller
 * of them. */
int maximumStrongPairXor(int *nums, int numsSize)
{
    int best = -1, i, j, lo;

    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsSize; j++) {
            lo = nums[i] < nums[j] ? nums[i] : nums[j];
            if (abs(nums[i] - nums[j]) <= lo && (nums[i] ^ nums[j]) > best)
                best = nums[i] ^ nums[j];
        }
    return best;
}
