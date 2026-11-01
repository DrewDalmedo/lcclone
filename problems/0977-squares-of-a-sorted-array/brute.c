/* Slow and obviously right: square every element, then sort the squares
 * by insertion sort. */
int *sortedSquares(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, j, v;

    for (i = 0; i < numsSize; i++) {
        v = nums[i] * nums[i];
        for (j = i; j > 0 && out[j - 1] > v; j--)
            out[j] = out[j - 1];
        out[j] = v;
    }
    *returnSize = numsSize;
    return out;
}
