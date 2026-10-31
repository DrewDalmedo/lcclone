/* Slow and obviously right: try every pair of indices. */
int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    int *out = malloc(2 * sizeof *out), i, k;

    for (i = 0; i < numsSize; i++)
        for (k = i + 1; k < numsSize; k++)
            if ((long long)nums[i] + nums[k] == target) {
                out[0] = i;
                out[1] = k;
                *returnSize = 2;
                return out;
            }
    *returnSize = 0;
    return out;
}
