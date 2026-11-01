/* Reference solution: take a bit that is set in some element x. Adding x
 * to a pick that lacks it, or taking it out of one that has it, flips that
 * bit of the XOR total, so it pairs the 2^n picks up and the bit is set in
 * exactly half of their totals. A bit set in no element is never set. So
 * the sum is (the OR of all elements) * 2^(n-1). */
int subsetXORSum(int *nums, int numsSize)
{
    int all = 0, i;

    for (i = 0; i < numsSize; i++)
        all |= nums[i];
    return all << (numsSize - 1);
}
