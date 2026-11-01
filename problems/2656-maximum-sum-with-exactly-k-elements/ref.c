/* Reference solution: always pick the counter showing the most. With M the
 * largest element, the k moves earn M, M + 1, ..., M + k - 1, which add up
 * to k * M + k * (k - 1) / 2. */
int maximizeSum(int *nums, int numsSize, int k)
{
    int best = nums[0], i;

    for (i = 1; i < numsSize; i++)
        if (nums[i] > best)
            best = nums[i];
    return k * best + k * (k - 1) / 2;
}
