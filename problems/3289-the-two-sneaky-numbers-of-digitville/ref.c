/* Reference solution: count each value in an array indexed by value
 * (values are below n = numsSize - 2), and keep a value the moment its
 * count reaches 2. The smaller of the two goes first. */
int *getSneakyNumbers(int *nums, int numsSize, int *returnSize)
{
    int *seen = calloc((size_t)numsSize, sizeof *seen);
    int *out = malloc(2 * sizeof *out), i, k = 0;

    for (i = 0; i < numsSize && k < 2; i++)
        if (++seen[nums[i]] == 2)
            out[k++] = nums[i];
    free(seen);
    if (k == 2 && out[0] > out[1]) {
        int t = out[0];

        out[0] = out[1];
        out[1] = t;
    }
    *returnSize = k;
    return out;
}
