/* Slow and obviously right: start from the front half alone, then slide
 * each number of the back half in right after its partner from the front
 * half, moving everything behind that spot one place along. */
int *shuffle(int *nums, int numsSize, int n, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int len = n, k, p;

    for (k = 0; k < n; k++)
        out[k] = nums[k];
    for (k = 0; k < n; k++) {
        /* out[0..2k] is merged already; nums[k] sits at 2k. */
        for (p = len; p > 2 * k + 1; p--)
            out[p] = out[p - 1];
        out[2 * k + 1] = nums[n + k];
        len++;
    }
    *returnSize = len;
    return out;
}
