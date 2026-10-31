/* Small cases: a few values from a narrow window (near 0 or near either
 * bound, so they repeat often), distinct values with at most one repeat,
 * values of either sign drawn from a few magnitudes (x and -x), or picks
 * from both bounds and their neighbours. Full-size cases: 10^5 distinct
 * values reaching both bounds (no repeat: the slowest case for a scan
 * that stops early), the same with one repeat at the two ends, side by
 * side at the end or anywhere, 5 * 10^4 pairs x and -x, or values from a
 * random range, which repeat when it is narrow. */

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

/* Copies one value onto another position, making a single repeat. */
static void repeat(int *a, int n)
{
    int p, q, where = (int)rnd(0, 2);

    if (where == 0) {
        p = 0;
        q = n - 1;
    } else if (where == 1) {
        p = n - 2;
        q = n - 1;
    } else {
        p = (int)rnd(0, n - 2);
        q = (int)rnd(p + 1, n - 1);
    }
    if (rnd_chance(50))
        a[q] = a[p];
    else
        a[p] = a[q];
}

static void small_case(int *a, int n)
{
    static const int edge[] = {-BOUND, -BOUND + 1, -1, 0, 1, BOUND - 1, BOUND};
    int shape = (int)rnd(0, 9), i;

    if (shape < 4) {
        int w = (int)rnd(0, 8), where = (int)rnd(0, 2);
        int lo = where == 0 ? (int)rnd(-5, 0)
               : where == 1 ? -BOUND : BOUND - w;

        for (i = 0; i < n; i++)
            a[i] = (int)rnd(lo, lo + w);
    } else if (shape < 7) {
        distinct(a, n);
        if (n > 1 && rnd_chance(50))
            repeat(a, n);
    } else if (shape < 9) {
        int far = rnd_chance(30);

        for (i = 0; i < n; i++) {
            int m = far ? BOUND - (int)rnd(0, 3) : (int)rnd(0, 4);

            a[i] = rnd_chance(50) ? m : -m;
        }
    } else {
        for (i = 0; i < n; i++)
            a[i] = edge[rnd(0, 6)];
    }
}

static void large_case(int *a, int n)
{
    int shape = (int)rnd(0, 9), i;

    if (shape < 4) {
        distinct(a, n);
    } else if (shape < 7) {
        distinct(a, n);
        repeat(a, n);
    } else if (shape < 9) {
        int half = n / 2;
        long long step = BOUND / half;

        for (i = 0; i < half; i++) {
            int m = (int)(1 + i * step + rnd(0, step - 1));

            if (i == half - 1)
                m = BOUND;
            a[2 * i] = m;
            a[2 * i + 1] = -m;
        }
        rnd_shuffle(a, n);
    } else {
        long long w = rnd(1, 2LL * BOUND);
        long long lo = rnd(-BOUND, BOUND - w);

        for (i = 0; i < n; i++)
            a[i] = (int)rnd(lo, lo + w);
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small)
        small_case(nums, n);
    else
        large_case(nums, n);
    put_ints(nums, n);
    free(nums);
}
