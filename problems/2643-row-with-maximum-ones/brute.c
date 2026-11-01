/* Slow and obviously right: try every possible count from n down to 0, and
 * for each scan the rows from the top, recounting their 1s; the first row
 * found with that count is the answer. */
int *rowAndMaximumOnes(int **mat, int matSize, int *matColSize,
                       int *returnSize)
{
    int n = matColSize[0], want, r, j, ones;
    int *ans = malloc(2 * sizeof *ans);

    *returnSize = 2;
    for (want = n; want >= 0; want--)
        for (r = 0; r < matSize; r++) {
            ones = 0;
            for (j = 0; j < n; j++)
                if (mat[r][j] == 1)
                    ones++;
            if (ones == want) {
                ans[0] = r;
                ans[1] = ones;
                return ans;
            }
        }
    ans[0] = ans[1] = -1; /* not reached: some count is in 0..n */
    return ans;
}
