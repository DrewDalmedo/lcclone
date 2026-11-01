/* Reference solution: XOR together every number from 0 to n and every
 * element. Each number that is in nums appears twice and cancels, so what
 * is left is the missing one. */
int missingNumber(int *nums, int numsSize)
{
    int r = numsSize, i;

    for (i = 0; i < numsSize; i++)
        r ^= i ^ nums[i];
    return r;
}
