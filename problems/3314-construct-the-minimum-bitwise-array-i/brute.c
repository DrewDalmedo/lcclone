/* Slow and obviously right: for each prime p, try a = 0, 1, 2, ... in turn
 * and keep the first one with a | (a + 1) == p. Since a | (a + 1) >= a + 1,
 * no a >= p can work, so the search stops at p - 1. */
int *minBitwiseArray(int *nums, int numsSize, int *returnSize)
{
    int *ans = malloc((size_t)numsSize * sizeof *ans);
    int i, a;

    for (i = 0; i < numsSize; i++) {
        ans[i] = -1;
        for (a = 0; a < nums[i]; a++)
            if ((a | (a + 1)) == nums[i]) {
                ans[i] = a;
                break;
            }
    }
    *returnSize = numsSize;
    return ans;
}
