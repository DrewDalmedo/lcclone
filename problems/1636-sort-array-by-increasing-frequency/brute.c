/* Slow and obviously right: insertion sort, where each comparison counts
 * both values afresh by scanning the whole input. */
static int count_of(const int *a, int n, int v)
{
    int i, c = 0;

    for (i = 0; i < n; i++)
        if (a[i] == v)
            c++;
    return c;
}

/* Nonzero when x must go before y. */
static int before(const int *a, int n, int x, int y)
{
    int cx = count_of(a, n, x), cy = count_of(a, n, y);

    if (cx != cy)
        return cx < cy;
    return x > y;
}

int *frequencySort(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, j, v;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        for (j = i; j > 0 && before(nums, numsSize, v, out[j - 1]); j--)
            out[j] = out[j - 1];
        out[j] = v;
    }
    *returnSize = numsSize;
    return out;
}
