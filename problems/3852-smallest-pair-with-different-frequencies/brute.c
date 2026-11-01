/* Slow and obviously right: try every pair of entries, count both values
 * by scanning the whole array, and keep the smallest working (x, y). */
static int occurrences(const int *nums, int n, int v)
{
    int c = 0, i;

    for (i = 0; i < n; i++)
        if (nums[i] == v)
            c++;
    return c;
}

int *minDistinctFreqPair(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc(2 * sizeof *out);
    int i, j, x, y;

    out[0] = out[1] = -1;
    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsSize; j++) {
            x = nums[i];
            y = nums[j];
            if (x >= y)
                continue;
            if (occurrences(nums, numsSize, x) ==
                occurrences(nums, numsSize, y))
                continue;
            if (out[0] == -1 || x < out[0] || (x == out[0] && y < out[1])) {
                out[0] = x;
                out[1] = y;
            }
        }
    *returnSize = 2;
    return out;
}
