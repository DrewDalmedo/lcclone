/* Reference solution: copy nums, then do the k steps one by one. Each step
 * scans for the smallest value, replacing the best only on a strictly
 * smaller one so that ties go to the lowest index, and multiplies it. With
 * k <= 10 and at most 100 elements, a scan per step is cheap; the largest
 * value reachable, 100 * 5^10 = 976562500, fits in an int. */
int *getFinalState(int *nums, int numsSize, int k, int multiplier,
                   int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int step, i, best;

    memcpy(out, nums, (size_t)numsSize * sizeof *out);
    for (step = 0; step < k; step++) {
        best = 0;
        for (i = 1; i < numsSize; i++)
            if (out[i] < out[best])
                best = i;
        out[best] *= multiplier;
    }
    *returnSize = numsSize;
    return out;
}
