/* Reference solution: one pass, OR-ing each even value into a result that
 * starts at 0, the answer when there is no even value. */
int evenNumberBitwiseORs(int *nums, int numsSize)
{
    int result = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] % 2 == 0)
            result |= nums[i];
    return result;
}
