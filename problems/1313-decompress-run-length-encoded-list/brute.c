/* Slow and obviously right: for each position of the result, walk the
 * pairs from the start to find the one whose copies cover it, and stop at
 * the first position that no pair covers. */
int *decompressRLElist(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc(5000 * sizeof *out);
    int pos = 0, before, i;

    for (;;) {
        before = 0;
        for (i = 0; i < numsSize; i += 2) {
            if (pos < before + nums[i])
                break;
            before += nums[i];
        }
        if (i >= numsSize)
            break;
        out[pos++] = nums[i + 1];
    }
    *returnSize = pos;
    return out;
}
