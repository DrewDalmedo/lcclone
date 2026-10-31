/* Reference solution: XOR all the elements. x ^ x is 0, so the two
 * copies of each paired value cancel, and only the lone value is left. */
int singleNumber(int *nums, int numsSize)
{
    int r = 0, i;

    for (i = 0; i < numsSize; i++)
        r ^= nums[i];
    return r;
}
