/* Reference solution: record that value v occurs by making nums[v - 1]
 * negative (reading each entry through abs(), since it may have been
 * marked already). Afterwards the entries still positive are at the
 * indices of the missing values. No memory besides the result. */
int *findDisappearedNumbers(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int n = 0, i, v;

    for (i = 0; i < numsSize; i++) {
        v = abs(nums[i]);
        if (nums[v - 1] > 0)
            nums[v - 1] = -nums[v - 1];
    }
    for (i = 0; i < numsSize; i++)
        if (nums[i] > 0)
            out[n++] = i + 1;
    *returnSize = n;
    return out;
}
