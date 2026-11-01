/* Reference solution: fill a new array in one pass, with nums[k] at
 * position 2k and nums[n + k] at position 2k + 1. */
int *shuffle(int *nums, int numsSize, int n, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int k;

    for (k = 0; k < n; k++) {
        out[2 * k] = nums[k];
        out[2 * k + 1] = nums[n + k];
    }
    *returnSize = 2 * n;
    return out;
}
