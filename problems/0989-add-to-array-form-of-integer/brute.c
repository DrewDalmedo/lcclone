/* Slow and obviously right: keep the number units first in a buffer and
 * add 1 to it, k times over, the way an odometer counts; then write the
 * digits out highest first, skipping zeros in front. */
int *addToArrayForm(int *num, int numSize, int k, int *returnSize)
{
    int cap = numSize + 6, len = numSize, i, j, step;
    int *d = calloc((size_t)cap, sizeof *d), *out;

    for (i = 0; i < numSize; i++)
        d[i] = num[numSize - 1 - i];
    for (step = 0; step < k; step++) {
        for (j = 0; d[j] == 9; j++)
            d[j] = 0;
        d[j]++;
        if (j + 1 > len)
            len = j + 1;
    }
    while (len > 1 && d[len - 1] == 0)
        len--;
    out = malloc((size_t)len * sizeof *out);
    for (i = 0; i < len; i++)
        out[i] = d[len - 1 - i];
    free(d);
    *returnSize = len;
    return out;
}
