/* Slow and obviously right: add up moves 0 to i anew for every i. */
int *runningSum(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    long long total;
    int i, k;

    for (i = 0; i < numsSize; i++) {
        total = 0;
        for (k = 0; k <= i; k++)
            total += nums[k];
        out[i] = (int)total;
    }
    *returnSize = numsSize;
    return out;
}
