/* Slow and obviously right: for each index i, look at every index j and
 * keep i when some nums[j] == key has |i - j| <= k. */
int *findKDistantIndices(int *nums, int numsSize, int key, int k,
                         int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int m = 0, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsSize; j++)
            if (nums[j] == key && abs(i - j) <= k) {
                out[m++] = i;
                break;
            }
    *returnSize = m;
    return out;
}
