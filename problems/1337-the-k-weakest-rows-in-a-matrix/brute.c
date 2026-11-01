/* Slow and obviously right: count the 1s of every row cell by cell, then
 * k times pick the weakest row not picked yet, comparing strengths first
 * and row numbers second. */
int *kWeakestRows(int **mat, int matSize, int *matColSize, int k,
                  int *returnSize)
{
    int *strength = malloc((size_t)matSize * sizeof *strength);
    char *picked = calloc((size_t)matSize, 1);
    int *out = malloc((size_t)k * sizeof *out);
    int i, j, t, best;

    for (i = 0; i < matSize; i++) {
        strength[i] = 0;
        for (j = 0; j < matColSize[i]; j++)
            strength[i] += mat[i][j];
    }
    for (t = 0; t < k; t++) {
        best = -1;
        for (i = 0; i < matSize; i++) {
            if (picked[i])
                continue;
            if (best < 0 || strength[i] < strength[best] ||
                (strength[i] == strength[best] && i < best))
                best = i;
        }
        picked[best] = 1;
        out[t] = best;
    }
    free(strength);
    free(picked);
    *returnSize = k;
    return out;
}
