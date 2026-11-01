/* Slow and obviously right: at each step, test every index against the
 * rule as stated: it is picked when no element is smaller and no element
 * before it is equal (so it is the first of the smallest). */
int *getFinalState(int *nums, int numsSize, int k, int multiplier,
                   int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int step, i, j, ok;

    for (i = 0; i < numsSize; i++)
        out[i] = nums[i];
    for (step = 0; step < k; step++)
        for (i = 0; i < numsSize; i++) {
            ok = 1;
            for (j = 0; j < numsSize; j++)
                if (out[j] < out[i] || (j < i && out[j] == out[i]))
                    ok = 0;
            if (ok) {
                out[i] *= multiplier;
                break;
            }
        }
    *returnSize = numsSize;
    return out;
}
