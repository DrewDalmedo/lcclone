/* Slow and obviously right: print each number with snprintf and copy the
 * characters, turned into digits, to the answer. */
int *separateDigits(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * 12 * sizeof *out);
    char buf[16];
    int len = 0, i, k;

    for (i = 0; i < numsSize; i++) {
        snprintf(buf, sizeof buf, "%d", nums[i]);
        for (k = 0; buf[k] != '\0'; k++)
            out[len++] = buf[k] - '0';
    }
    *returnSize = len;
    return out;
}
