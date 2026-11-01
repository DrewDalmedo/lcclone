/* Reference solution: no sorting needed. If `below` elements are smaller
 * than target and `equal` elements equal it, the sorted array holds target
 * at positions below, below + 1, ..., below + equal - 1. */
int *targetIndices(int *nums, int numsSize, int target, int *returnSize)
{
    int below = 0, equal = 0, i, *out;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] < target)
            below++;
        else if (nums[i] == target)
            equal++;
    }
    out = malloc((size_t)(equal > 0 ? equal : 1) * sizeof *out);
    for (i = 0; i < equal; i++)
        out[i] = below + i;
    *returnSize = equal;
    return out;
}
