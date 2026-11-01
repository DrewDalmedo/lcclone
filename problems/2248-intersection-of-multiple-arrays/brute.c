/* Slow and obviously right: take each number of the first list, look for
 * it in every other list one number at a time, keep it if every list has
 * it, and sort the kept numbers by repeatedly picking the smallest. */
int *intersection(int **nums, int numsSize, int *numsColSize, int *returnSize)
{
    int *out = malloc((size_t)numsColSize[0] * sizeof *out);
    int m = 0, i, j, k, v, found, everywhere, t;

    for (j = 0; j < numsColSize[0]; j++) {
        v = nums[0][j];
        everywhere = 1;
        for (i = 1; i < numsSize && everywhere; i++) {
            found = 0;
            for (k = 0; k < numsColSize[i]; k++)
                if (nums[i][k] == v)
                    found = 1;
            everywhere = found;
        }
        if (everywhere)
            out[m++] = v;
    }
    for (i = 0; i < m; i++)
        for (k = i + 1; k < m; k++)
            if (out[k] < out[i]) {
                t = out[i];
                out[i] = out[k];
                out[k] = t;
            }
    *returnSize = m;
    return out;
}
