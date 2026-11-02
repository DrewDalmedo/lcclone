/* Slow and obviously right: append nums to the answer, then append a copy
 * of nums that was reversed by swapping its ends inward. */
int *concatWithReverse(int *nums, int numsSize, int *returnSize)
{
    int *ans = malloc(2 * (size_t)numsSize * sizeof *ans);
    int *back = malloc((size_t)numsSize * sizeof *back);
    int len = 0, lo, hi, t, i;

    for (i = 0; i < numsSize; i++)
        back[i] = nums[i];
    for (lo = 0, hi = numsSize - 1; lo < hi; lo++, hi--) {
        t = back[lo];
        back[lo] = back[hi];
        back[hi] = t;
    }
    for (i = 0; i < numsSize; i++)
        ans[len++] = nums[i];
    for (i = 0; i < numsSize; i++)
        ans[len++] = back[i];
    free(back);
    *returnSize = len;
    return ans;
}
