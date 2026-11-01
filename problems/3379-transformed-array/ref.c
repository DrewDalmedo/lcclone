/* Reference solution: the token from square i stops on square
 * (i + nums[i]) mod n, taking the remainder that is 0 or more. C's %
 * keeps the sign of a negative left side, so add n once more. */
int *constructTransformedArray(int *nums, int numsSize, int *returnSize)
{
    int n = numsSize, i, j;
    int *res = malloc((size_t)n * sizeof *res);

    for (i = 0; i < n; i++) {
        j = ((i + nums[i]) % n + n) % n;
        res[i] = nums[j];
    }
    *returnSize = n;
    return res;
}
