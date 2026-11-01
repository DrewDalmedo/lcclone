/* A grid of numbers of every length from 1 to 11 characters. Each number
 * gets a random digit count, often the smallest or largest value with that
 * many digits (9, 10, 99, 100, ...), and is negative half the time; zero,
 * 10^9 and -10^9 come up too. Small grids are at most 5 x 5, and their
 * digit counts are drawn from a narrow range so that columns tie often;
 * full-size ones are 100 x 100, 100 x 1 or 1 x 100. */
static int number(int lo_digits, int hi_digits)
{
    static const int p10[] = {1, 10, 100, 1000, 10000, 100000, 1000000,
                                10000000, 100000000, 1000000000};
    int d = (int)rnd(lo_digits, hi_digits);
    int kind = (int)rnd(0, 9);
    int v;

    if (kind == 0)
        return 0;
    if (d == 10)
        v = 1000000000;
    else if (kind == 1)
        v = d == 1 ? 1 : p10[d - 1]; /* smallest with d digits */
    else if (kind == 2)
        v = p10[d] - 1; /* largest with d digits */
    else
        v = (int)rnd(d == 1 ? 1 : p10[d - 1], p10[d] - 1);
    if (rnd_chance(50))
        v = -v;
    return v;
}

void gen(int small)
{
    int m, n, shape, lo, hi, i;
    int *a;

    if (small) {
        m = (int)rnd(1, 5);
        n = (int)rnd(1, 5);
        lo = (int)rnd(1, 10);
        hi = (int)rnd(lo, lo + 2 > 10 ? 10 : lo + 2);
    } else {
        shape = (int)rnd(0, 2);
        m = shape == 2 ? 1 : 100;
        n = shape == 1 ? 1 : 100;
        lo = 1;
        hi = 10;
    }
    a = malloc((size_t)m * n * sizeof *a);
    for (i = 0; i < m * n; i++)
        a[i] = number(lo, hi);
    put_int_grid(a, m, n);
    free(a);
}
