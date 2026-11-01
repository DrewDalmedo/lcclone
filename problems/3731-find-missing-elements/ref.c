/* Reference solution: mark the values of nums in a table indexed by
 * value (up to 100, so 101 slots), note the smallest and the largest,
 * and list the unmarked values between them in increasing order. The
 * answer can be longer than nums, so its buffer is sized by the range. */
int *findMissingElements(int *nums, int numsSize, int *returnSize)
{
    bool seen[101] = {false};
    int lo = nums[0], hi = nums[0], m = 0, i, v;
    int *out;

    for (i = 0; i < numsSize; i++) {
        seen[nums[i]] = true;
        if (nums[i] < lo)
            lo = nums[i];
        if (nums[i] > hi)
            hi = nums[i];
    }
    out = malloc((size_t)(hi - lo + 1) * sizeof *out);
    for (v = lo + 1; v < hi; v++)
        if (!seen[v])
            out[m++] = v;
    *returnSize = m;
    return out;
}
