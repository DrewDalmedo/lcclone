/* Slow and obviously right: search every sequence of squashes, shortest
 * first (iterative deepening), until one leaves all entries equal. */
static int all_equal(const int *a, int n)
{
    int i;

    for (i = 1; i < n; i++)
        if (a[i] != a[0])
            return 0;
    return 1;
}

/* Whether at most `left` more squashes can make a[0..n) all equal. */
static int reachable(const int *a, int n, int left)
{
    int *b, l, r, i, and, ok = 0;

    if (all_equal(a, n))
        return 1;
    if (left == 0)
        return 0;
    b = malloc((size_t)n * sizeof *b);
    for (l = 0; l < n && !ok; l++)
        for (r = l; r < n && !ok; r++) {
            and = a[l];
            for (i = l + 1; i <= r; i++)
                and &= a[i];
            for (i = 0; i < n; i++)
                b[i] = (i >= l && i <= r) ? and : a[i];
            ok = reachable(b, n, left - 1);
        }
    free(b);
    return ok;
}

int minOperations(int *nums, int numsSize)
{
    int depth = 0;

    while (!reachable(nums, numsSize, depth))
        depth++;
    return depth;
}
