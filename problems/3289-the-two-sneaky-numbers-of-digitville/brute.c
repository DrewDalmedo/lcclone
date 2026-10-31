/* Slow and obviously right: compare every pair of positions, and keep the
 * value of each pair that holds equal values. */
int *getSneakyNumbers(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out), i, j, k = 0;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] == nums[j])
                out[k++] = nums[i];
    *returnSize = k;
    return out;
}
