/* Reference solution: while some apple is lighter than k, the lightest
 * one is, so every step removes one apple below k and never one of k or
 * more. The answer is the number of weights below k. */
int minOperations(int *nums, int numsSize, int k)
{
    int count = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] < k)
            count++;
    return count;
}
