/* Reference solution: add up the counts to size the result, then write
 * out the copies of each pair in turn. */
int *decompressRLElist(int *nums, int numsSize, int *returnSize)
{
    int total = 0, len = 0, i, k;
    int *out;

    for (i = 0; i < numsSize; i += 2)
        total += nums[i];
    out = malloc((size_t)total * sizeof *out);
    for (i = 0; i < numsSize; i += 2)
        for (k = 0; k < nums[i]; k++)
            out[len++] = nums[i + 1];
    *returnSize = total;
    return out;
}
