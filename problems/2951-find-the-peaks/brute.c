/* Slow and obviously right: for every index, look at every other index
 * within distance 1, and count it a peak only when it has two of them and
 * is taller than each. */
int *findPeaks(int *mountain, int mountainSize, int *returnSize)
{
    int *out = malloc((size_t)mountainSize * sizeof *out), i, j, k = 0;

    for (i = 0; i < mountainSize; i++) {
        int near = 0, taller = 1;

        for (j = 0; j < mountainSize; j++)
            if (j != i && (j == i - 1 || j == i + 1)) {
                near++;
                if (mountain[j] >= mountain[i])
                    taller = 0;
            }
        if (near == 2 && taller)
            out[k++] = i;
    }
    *returnSize = k;
    return out;
}
