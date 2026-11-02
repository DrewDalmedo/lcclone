/* Reference solution: copy nums into the result one element at a time.
 * An element is kept unless the result already ends in k copies of it;
 * since equal values are adjacent, that is when the element k places back
 * in the result (not in nums) has the same value. */
int *limitOccurrences(int *nums, int numsSize, int k, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int w = 0, i;

    for (i = 0; i < numsSize; i++)
        if (w < k || out[w - k] != nums[i])
            out[w++] = nums[i];
    *returnSize = w;
    return out;
}
