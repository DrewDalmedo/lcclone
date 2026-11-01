/* Small cases: k from 0 to a little past n (sometimes far past it), and a
 * few values from a narrow window (near 0 or near either bound, so they
 * repeat often), distinct values with one repeat at distance exactly k or
 * k + 1, values of either sign drawn from a few magnitudes (x and -x), or
 * picks from both bounds and their neighbours.
 * Full-size cases: 10^5 distinct values reaching both bounds with k at
 * least n - 1; k + 1 distinct values repeated over and over, so every
 * repeat is k + 1 apart, sometimes with one at distance exactly k at the
 * very end (both slow for checking k positions after each one); one value
 * every k + 1 positions and then once more at distance k or less (wrong
 * for a solution that keeps a value's first position); or values from a
 * random range with a random k. */

#define BOUND 1000000000

/* n distinct values in random order: one in each of n equal slices of
 * [-BOUND, BOUND], the first and last slices pinned to the bounds. */
static void distinct(int *a, int n)
{
    long long step = 2LL * BOUND / n;
    int i;

    for (i = 0; i < n; i++)
        a[i] = (int)(-BOUND + i * step + rnd(0, step - 1));
    if (n > 1) {
        a[0] = -BOUND;
        a[n - 1] = BOUND;
    }
    rnd_shuffle(a, n);
}

static int small_case(int *a, int n)
{
    static const int edge[] = {-BOUND, -BOUND + 1, -1, 0, 1, BOUND - 1, BOUND};
    int k = rnd_chance(85) ? (int)rnd(0, n) : (int)rnd(n, 100000);
    int shape = (int)rnd(0, 9), i;

    if (shape < 5) {
        int w = (int)rnd(0, 4), where = (int)rnd(0, 2);
        int lo = where == 0 ? (int)rnd(-3, 0)
               : where == 1 ? -BOUND : BOUND - w;

        for (i = 0; i < n; i++)
            a[i] = (int)rnd(lo, lo + w);
    } else if (shape < 7) {
        int d = (int)rnd(k, k + 1), p;

        distinct(a, n);
        if (d >= 1 && d <= n - 1) {
            p = (int)rnd(0, n - 1 - d);
            a[p + d] = a[p];
        }
    } else if (shape < 9) {
        int far = rnd_chance(30);

        for (i = 0; i < n; i++) {
            int m = far ? BOUND - (int)rnd(0, 2) : (int)rnd(0, 3);

            a[i] = rnd_chance(50) ? m : -m;
        }
    } else {
        for (i = 0; i < n; i++)
            a[i] = edge[rnd(0, 6)];
    }
    return k;
}

static int large_case(int *a, int n)
{
    int shape = (int)rnd(0, 9), k, i;

    if (shape < 3) {
        k = (int)rnd(n - 1, 100000);
        distinct(a, n);
    } else if (shape < 6) {
        int *v;

        k = (int)rnd(n / 2, n - 2);
        v = malloc((size_t)(k + 1) * sizeof *v);
        distinct(v, k + 1);
        for (i = 0; i < n; i++)
            a[i] = v[i % (k + 1)];
        if (rnd_chance(50))
            a[n - 1] = a[n - 1 - k];
        free(v);
    } else if (shape < 8) {
        int pos = 0, v;

        k = (int)rnd(1, 2000);
        distinct(a, n);
        v = a[0];
        while (pos + 2 * k + 1 <= n - 1) {
            pos += k + 1;
            a[pos] = v;
        }
        a[pos + (int)rnd(1, k)] = v;
    } else {
        long long w = rnd(1, 2LL * BOUND);
        long long lo = rnd(-BOUND, BOUND - w);

        k = rnd_chance(25) ? 0 : (int)rnd(0, 100000);
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(lo, lo + w);
    }
    return k;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, k;
    int *nums = malloc((size_t)n * sizeof *nums);

    k = small ? small_case(nums, n) : large_case(nums, n);
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
