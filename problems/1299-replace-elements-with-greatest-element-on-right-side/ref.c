/* Reference solution: walk from right to left, keeping the largest value
 * seen so far, which starts at -1; each position gets that maximum before
 * its own value joins it. */
int *replaceElements(int *arr, int arrSize, int *returnSize)
{
    int *out = malloc((size_t)arrSize * sizeof *out);
    int best = -1, i;

    for (i = arrSize - 1; i >= 0; i--) {
        out[i] = best;
        if (arr[i] > best)
            best = arr[i];
    }
    *returnSize = arrSize;
    return out;
}
