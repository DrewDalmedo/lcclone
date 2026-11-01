/* Slow and obviously right: walk the token one square at a time, up or
 * down, wrapping at the ends of the loop. */
int *constructTransformedArray(int *nums, int numsSize, int *returnSize)
{
    int n = numsSize, i, pos, step;
    int *res = malloc((size_t)n * sizeof *res);

    for (i = 0; i < n; i++) {
        pos = i;
        for (step = 0; step < nums[i]; step++)
            pos = pos == n - 1 ? 0 : pos + 1;
        for (step = 0; step < -nums[i]; step++)
            pos = pos == 0 ? n - 1 : pos - 1;
        res[i] = nums[pos];
    }
    *returnSize = n;
    return res;
}
