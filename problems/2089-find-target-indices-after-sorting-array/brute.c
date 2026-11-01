/* Slow and obviously right: sort a copy of nums by insertion and list
 * the positions where it holds target. */
int *targetIndices(int *nums, int numsSize, int target, int *returnSize)
{
    int sorted[100], *out = malloc((size_t)numsSize * sizeof *out);
    int m = 0, i, k, v;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        for (k = i; k > 0 && sorted[k - 1] > v; k--)
            sorted[k] = sorted[k - 1];
        sorted[k] = v;
    }
    for (i = 0; i < numsSize; i++)
        if (sorted[i] == target)
            out[m++] = i;
    *returnSize = m;
    return out;
}
