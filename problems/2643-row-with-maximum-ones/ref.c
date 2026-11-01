/* Reference solution: count the 1s of each row in one pass, and replace
 * the best row only when a later row has strictly more, so the earliest of
 * tied rows stays. */
int *rowAndMaximumOnes(int **mat, int matSize, int *matColSize,
                       int *returnSize)
{
    int n = matColSize[0], r, j, ones;
    int *ans = malloc(2 * sizeof *ans);

    ans[0] = 0;
    ans[1] = -1;
    for (r = 0; r < matSize; r++) {
        ones = 0;
        for (j = 0; j < n; j++)
            ones += mat[r][j];
        if (ones > ans[1]) {
            ans[0] = r;
            ans[1] = ones;
        }
    }
    *returnSize = 2;
    return ans;
}
