/* Reference solution: for each number find its highest power of ten,
 * then take the digits off from the top, dividing by that power and
 * stepping it down by a factor of ten. A number has at most 6 digits, so
 * 6 slots per number are enough. */
int *separateDigits(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * 6 * sizeof *out);
    int len = 0, i, x, p;

    for (i = 0; i < numsSize; i++) {
        x = nums[i];
        p = 1;
        while (p <= x / 10)
            p *= 10;
        for (; p > 0; p /= 10) {
            out[len++] = x / p;
            x %= p;
        }
    }
    *returnSize = len;
    return out;
}
