/* Slow and obviously right: keep nums[i] when fewer than k of the elements
 * before it in nums are equal to it, counting them anew for every i. */
int *limitOccurrences(int *nums, int numsSize, int k, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int m = 0, i, j, before;

    for (i = 0; i < numsSize; i++) {
        before = 0;
        for (j = 0; j < i; j++)
            if (nums[j] == nums[i])
                before++;
        if (before < k)
            out[m++] = nums[i];
    }
    *returnSize = m;
    return out;
}
