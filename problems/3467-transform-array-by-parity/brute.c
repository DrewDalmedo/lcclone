/* Straight from the definition: replace each element by its remainder
 * mod 2, then sort the marks with a plain insertion sort. */
int *transformArray(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, j, v;

    for (i = 0; i < numsSize; i++)
        out[i] = nums[i] % 2;
    for (i = 1; i < numsSize; i++) {
        v = out[i];
        for (j = i; j > 0 && out[j - 1] > v; j--)
            out[j] = out[j - 1];
        out[j] = v;
    }
    *returnSize = numsSize;
    return out;
}
