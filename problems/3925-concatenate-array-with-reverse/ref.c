/* Reference solution: one pass writes each nums[i] to both of its places,
 * i and 2n - 1 - i, in a new array of 2n. */
int *concatWithReverse(int *nums, int numsSize, int *returnSize)
{
    int n = numsSize, i;
    int *ans = malloc(2 * (size_t)n * sizeof *ans);

    for (i = 0; i < n; i++) {
        ans[i] = nums[i];
        ans[2 * n - 1 - i] = nums[i];
    }
    *returnSize = 2 * n;
    return ans;
}
