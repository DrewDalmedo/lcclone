/* Reference solution: count the even elements, then fill the result with
 * that many 0s followed by 1s for the rest. */
int *transformArray(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int evens = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] % 2 == 0)
            evens++;
    for (i = 0; i < numsSize; i++)
        out[i] = i < evens ? 0 : 1;
    *returnSize = numsSize;
    return out;
}
