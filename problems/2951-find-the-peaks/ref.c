/* Reference solution: one pass over the inner indices, keeping those
 * taller than both neighbours. */
int *findPeaks(int *mountain, int mountainSize, int *returnSize)
{
    int *out = malloc((size_t)mountainSize * sizeof *out), i, k = 0;

    for (i = 1; i + 1 < mountainSize; i++)
        if (mountain[i] > mountain[i - 1] && mountain[i] > mountain[i + 1])
            out[k++] = i;
    *returnSize = k;
    return out;
}
