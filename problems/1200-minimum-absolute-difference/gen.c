/* Distinct numbers, shuffled. Small cases have 2 to 8 numbers, mostly
 * from -12..12 so that gaps tie often; some are arithmetic progressions
 * (every gap is the smallest), and some spread over the whole range, the
 * bounds included. Full-size ones are 10^5 numbers, with the seed picking the
 * shape: random over the whole range, bounds included; a progression
 * (99999 pairs); a progression of step 20 with one gap of 1 (one pair);
 * or random numbers sorted in decreasing order. */
#define LO (-1000000)
#define HI 1000000

/* Fills a[0..n) with distinct random numbers from lo..hi, after the
 * first `have` that are already there. */
static void distinct(int *a, int have, int n, int lo, int hi)
{
    char *seen = calloc((size_t)(hi - lo + 1), 1);
    int i, v;

    for (i = 0; i < have; i++)
        seen[a[i] - lo] = 1;
    for (i = have; i < n; i++) {
        do
            v = (int)rnd(lo, hi);
        while (seen[v - lo]);
        seen[v - lo] = 1;
        a[i] = v;
    }
    free(seen);
}

static int cmp_desc(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

void gen(int small)
{
    int n = 100000, *a;
    int shape = small ? (int)rnd(0, 9) : (int)rnd(0, 3);
    int i, step, start, have = 0;

    if (small)
        n = rnd_chance(15) ? 2 : (int)rnd(3, 8);
    a = malloc((size_t)n * sizeof *a);
    if (small && shape <= 5) {
        distinct(a, 0, n, -12, 12);
    } else if (small && shape <= 7) {
        step = (int)rnd(1, 4);
        start = (int)rnd(-10, 10);
        for (i = 0; i < n; i++)
            a[i] = start + i * step;
    } else if (small) {
        if (rnd_chance(50))
            a[have++] = LO;
        if (rnd_chance(50))
            a[have++] = HI;
        distinct(a, have, n, LO, HI);
    } else if (shape == 0 || shape == 3) {
        a[0] = LO;
        a[1] = HI;
        distinct(a, 2, n, LO, HI);
    } else if (shape == 1) {
        step = (int)rnd(1, (HI - LO) / (n - 1));
        start = (int)rnd(LO, HI - step * (n - 1));
        for (i = 0; i < n; i++)
            a[i] = start + i * step;
    } else {
        for (i = 0; i < n; i++)
            a[i] = LO + 20 * i;
        i = (int)rnd(1, n - 1);
        a[i] = a[i - 1] + 1;
    }
    if (!small && shape == 3)
        qsort(a, (size_t)n, sizeof *a, cmp_desc);
    else
        rnd_shuffle(a, n);
    put_ints(a, n);
    free(a);
}
