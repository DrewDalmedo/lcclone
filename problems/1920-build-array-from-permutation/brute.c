/* Obviously right: straight from the definition, into a new array, leaving
 * nums untouched: from each i take two steps along the arrows. */
int *buildArray(int *nums, int numsSize, int *returnSize)
{
    int *ans = malloc((size_t)numsSize * sizeof *ans);
    int i, at, s;

    for (i = 0; i < numsSize; i++) {
        at = i;
        for (s = 0; s < 2; s++)
            at = nums[at];
        ans[i] = at;
    }
    *returnSize = numsSize;
    return ans;
}
