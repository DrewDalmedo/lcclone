/* Numbers and a threshold k. Small cases have 1 to 8 numbers from 0 to 9,
 * or now and then from 10^9 - 9 to 10^9; full-size cases have 100 numbers
 * up to 10^9. The seed picks the shape:
 *   - random numbers;
 *   - falling numbers: every cut has the same gap, so the answer is 0 or
 *     -1;
 *   - the smallest number last but one and the largest last, with k below
 *     the gap of the first cut, so that only the last cut is stable;
 *   - the largest number last and k = 0.
 * For the first two shapes k is usually the gap of a random cut, or one
 * less, so that ties with k come up often; otherwise it is random, or 0. */
static int gap(const int *a, int n, int i)
{
    int hi = a[0], lo = a[i], j;

    for (j = 0; j <= i; j++)
        if (a[j] > hi)
            hi = a[j];
    for (j = i; j < n; j++)
        if (a[j] < lo)
            lo = a[j];
    return hi - lo;
}

static int cmp_desc(const void *x, const void *y)
{
    int a = *(const int *)x, b = *(const int *)y;

    return (a < b) - (a > b);
}

static void swap_to(int *a, int from, int to)
{
    int t = a[from];

    a[from] = a[to];
    a[to] = t;
}

void gen(int small)
{
    static const int small_shape[] = {0, 0, 0, 1, 2, 3};
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? small_shape[rnd(0, 5)] : (int)rnd(0, 3);
    int big = small && rnd_chance(15);
    int lo = big ? 999999991 : 0, hi = small ? lo + 9 : 1000000000;
    int *a = malloc((size_t)n * sizeof *a);
    int i, j, k, pick;

    if (shape == 2 && n < 3)
        shape = 0;
    for (i = 0; i < n; i++)
        a[i] = (int)rnd(lo, hi);
    if (shape == 1)
        qsort(a, (size_t)n, sizeof *a, cmp_desc);
    if (shape == 2) {
        for (i = 1, j = 0; i < n; i++)
            if (a[i] > a[j])
                j = i;
        swap_to(a, j, n - 1);
        for (i = 1, j = 0; i < n - 1; i++)
            if (a[i] < a[j])
                j = i;
        swap_to(a, j, n - 2);
    }
    if (shape == 3)
        a[n - 1] = hi;
    pick = (int)rnd(0, 9);
    if (shape == 2) {
        k = a[0] > a[n - 2] ? (int)rnd(0, a[0] - a[n - 2] - 1) : 0;
    } else if (shape == 3 || pick == 0) {
        k = 0;
    } else if (pick == 1) {
        k = (int)rnd(0, small ? 10 : 1000000000);
    } else {
        k = gap(a, n, (int)rnd(0, n - 1));
        if (pick >= 6 && k > 0)
            k--;
    }
    put_ints(a, n);
    put_int(k);
    free(a);
}
