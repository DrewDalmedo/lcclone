/* Obviously right, by another route: walk 2n positions and read nums
 * round and round, position k taking nums[k mod n]. */
int *getConcatenation(int *nums, int numsSize, int *returnSize)
{
    int *ans = malloc(2 * (size_t)numsSize * sizeof *ans);
    int k;

    for (k = 0; k < 2 * numsSize; k++)
        ans[k] = nums[k % numsSize];
    *returnSize = 2 * numsSize;
    return ans;
}
