/* Reference solution: one pass. i >> 1 is i with its lowest bit dropped,
 * and it is smaller than i, so its count is already known: add back 1 if
 * the dropped bit was set. */
int *countBits(int n, int *returnSize)
{
    int *ans = malloc((size_t)(n + 1) * sizeof *ans);
    int i;

    ans[0] = 0;
    for (i = 1; i <= n; i++)
        ans[i] = ans[i >> 1] + (i & 1);
    *returnSize = n + 1;
    return ans;
}
