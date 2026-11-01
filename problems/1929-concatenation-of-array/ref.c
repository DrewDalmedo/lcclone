/* Reference solution: copy nums into the first half of a new array of 2n
 * and again into the second half. */
int *getConcatenation(int *nums, int numsSize, int *returnSize)
{
    int *ans = malloc(2 * (size_t)numsSize * sizeof *ans);

    memcpy(ans, nums, (size_t)numsSize * sizeof *ans);
    memcpy(ans + numsSize, nums, (size_t)numsSize * sizeof *ans);
    *returnSize = 2 * numsSize;
    return ans;
}
