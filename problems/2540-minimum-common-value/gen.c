/* Two sorted logs. Small cases hold 1 to 8 timestamps each from a narrow
 * range, so that common values, repeats and misses all come up; now and
 * then the range ends at 10^9, or the two logs use ranges that don't
 * overlap. Full-size ones hold 10^5 timestamps each, shaped by the seed:
 * interleaved with nothing in common, interleaved with a single common
 * value 10^9 at the very end of both, ranges that don't overlap, one
 * timestamp against 10^5 that matches only the last of them, or many
 * common values. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

static void narrow(int *a, int n, int lo, int width)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = lo + (int)rnd(0, width);
    qsort(a, (size_t)n, sizeof *a, cmp_int);
}

/* a[i] = mul * u + add, where u starts at start and grows by 0 to step
 * at each entry, so a is non-decreasing with repeats. */
static void walk(int *a, int n, int start, int step, int mul, int add)
{
    int u = start, i;

    for (i = 0; i < n; i++) {
        a[i] = mul * u + add;
        u += (int)rnd(0, step);
    }
}

void gen(int small)
{
    int n, m, shape, width, lo, swap, *a, *b, *t;

    if (small) {
        n = (int)rnd(1, 8);
        m = (int)rnd(1, 8);
        width = (int)rnd(1, 15);
        lo = rnd_chance(15) ? 1000000000 - 2 * width - 1 : 1;
        a = malloc((size_t)n * sizeof *a);
        b = malloc((size_t)m * sizeof *b);
        narrow(a, n, lo, width);
        if (rnd_chance(15))
            narrow(b, m, lo + width + 1, width);
        else
            narrow(b, m, lo, width);
    } else {
        shape = (int)rnd(0, 4);
        n = shape == 3 ? 1 : 100000;
        m = 100000;
        a = malloc((size_t)n * sizeof *a);
        b = malloc((size_t)m * sizeof *b);
        if (shape <= 1) {
            walk(a, n, 1, 4000, 2, 0);
            walk(b, m, 0, 4000, 2, 1);
            if (shape == 1)
                a[n - 1] = b[m - 1] = 1000000000;
        } else if (shape == 2) {
            walk(a, n, 1, 4999, 1, 0);
            walk(b, m, 500000001, 4999, 1, 0);
        } else if (shape == 3) {
            walk(b, m, 1, 4000, 2, 0);
            a[0] = b[m - 1];
        } else {
            walk(a, n, 1, 3, 2, 0);
            walk(b, m, 1, 3, 2, 0);
        }
    }
    swap = (int)rnd(0, 1);
    if (swap) {
        t = a;
        a = b;
        b = t;
        swap = n;
        n = m;
        m = swap;
    }
    put_ints(a, n);
    put_ints(b, m);
    free(a);
    free(b);
}
