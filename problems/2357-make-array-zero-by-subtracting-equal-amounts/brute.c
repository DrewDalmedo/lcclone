/* Slow and obviously right: play every possible round, trying each x from
 * 1 to m, and keep the shortest way to all zeros. Exponential in the
 * values, so only for the small cases of gen.c (values up to 9). */
static int least_rounds(int *a, int n)
{
    int m = 0, best = -1, x, i, r;
    int b[100];

    for (i = 0; i < n; i++)
        if (a[i] > 0 && (m == 0 || a[i] < m))
            m = a[i];
    if (m == 0)
        return 0;
    for (x = 1; x <= m; x++) {
        for (i = 0; i < n; i++)
            b[i] = a[i] > 0 ? a[i] - x : 0;
        r = 1 + least_rounds(b, n);
        if (best < 0 || r < best)
            best = r;
    }
    return best;
}

int minimumOperations(int *nums, int numsSize)
{
    return least_rounds(nums, numsSize);
}
