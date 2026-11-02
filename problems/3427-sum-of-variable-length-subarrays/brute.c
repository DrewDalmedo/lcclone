/* Slow and obviously right: for each element, walk back from it one
 * element at a time, taking itself and up to nums[i] elements before it. */
int subarraySum(int *nums, int numsSize)
{
    int total = 0, i, j, seen;

    for (i = 0; i < numsSize; i++)
        for (j = i, seen = 0; j >= 0 && seen <= nums[i]; j--, seen++)
            total += nums[j];
    return total;
}
