/* Reference solution: find each row's strength with a binary search for
 * its first 0, then a counting sort by strength. Rows of equal strength
 * are placed in increasing row order, so ties are broken by the row
 * number; the first k placed rows are the answer. */
int *kWeakestRows(int **mat, int matSize, int *matColSize, int k,
                  int *returnSize)
{
    int m = matSize, n = matColSize[0], i, s, lo, hi, mid;
    int *strength = malloc((size_t)m * sizeof *strength);
    int *start = calloc((size_t)n + 2, sizeof *start);
    int *order = malloc((size_t)m * sizeof *order);

    for (i = 0; i < m; i++) {
        lo = 0; /* mat[i][0 .. lo) are 1s */
        hi = n; /* mat[i][hi .. n) are 0s */
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (mat[i][mid] == 1)
                lo = mid + 1;
            else
                hi = mid;
        }
        strength[i] = lo;
        start[lo + 1]++;
    }
    for (s = 0; s < n; s++) /* start[s] = rows weaker than strength s */
        start[s + 1] += start[s];
    for (i = 0; i < m; i++)
        order[start[strength[i]]++] = i;
    free(strength);
    free(start);
    *returnSize = k;
    return order;
}
