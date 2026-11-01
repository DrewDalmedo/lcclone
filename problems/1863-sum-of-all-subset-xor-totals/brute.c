/* Slow and obviously right: go through all 2^n picks as bitmasks, work out
 * each one's XOR total, and add them up. */
int subsetXORSum(int *nums, int numsSize)
{
    int sum = 0, mask, total, i;

    for (mask = 0; mask < (1 << numsSize); mask++) {
        total = 0;
        for (i = 0; i < numsSize; i++)
            if (mask & (1 << i))
                total ^= nums[i];
        sum += total;
    }
    return sum;
}
