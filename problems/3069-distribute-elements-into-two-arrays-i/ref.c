/* Reference solution: simulate the two rows. Each row is kept in its own
 * buffer of n ints (either can end up holding n - 1 numbers), and each
 * placement compares the two rows' last numbers. The top row is then copied
 * into the result, followed by the bottom row. */
int *resultArray(int *nums, int numsSize, int *returnSize)
{
    int *top = malloc((size_t)numsSize * sizeof *top);
    int *bottom = malloc((size_t)numsSize * sizeof *bottom);
    int *out = malloc((size_t)numsSize * sizeof *out);
    int nt = 0, nb = 0, i;

    top[nt++] = nums[0];
    bottom[nb++] = nums[1];
    for (i = 2; i < numsSize; i++) {
        if (top[nt - 1] > bottom[nb - 1])
            top[nt++] = nums[i];
        else
            bottom[nb++] = nums[i];
    }
    memcpy(out, top, (size_t)nt * sizeof *out);
    memcpy(out + nt, bottom, (size_t)nb * sizeof *out);
    free(top);
    free(bottom);
    *returnSize = numsSize;
    return out;
}
