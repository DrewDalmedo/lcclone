/* Random values, usually with a planted repetition: k - 1, k or k + 1
 * equal groups of m in a row from a random start (often the far end),
 * sometimes with one value spoiled. Other cases are periodic arrays, or
 * any m and k, which often ask for more than fits (m * k > n, even
 * m >= n). Small cases draw from a few values so that repeats come up by
 * chance too; full-size ones have 100 elements. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 10) : 100;
    int top = small ? (int)rnd(1, 3) : (int)rnd(1, 100);
    int shape = (int)rnd(0, 4), m, k, c, start, period, pos, i, t;
    int *a = malloc((size_t)n * sizeof *a);

    for (i = 0; i < n; i++)
        a[i] = (int)rnd(1, top);
    if (shape == 0) {
        m = (int)rnd(1, small ? n + 2 : 100);
        k = (int)rnd(2, small ? 6 : 100);
    } else if (shape == 1) {
        period = (int)rnd(1, 4);
        for (i = period; i < n; i++)
            a[i] = a[i - period];
        m = (int)rnd(1, n / 2);
        k = (int)rnd(2, n / m < 100 ? n / m + 1 : 100);
    } else {
        m = (int)rnd(1, n / 2);
        k = (int)rnd(2, n / m);
        c = k + (int)rnd(-1, 1);
        if (c * m > n)
            c = k;
        start = rnd_chance(30) ? n - c * m : (int)rnd(0, n - c * m);
        for (t = 1; t < c; t++)
            for (i = 0; i < m; i++)
                a[start + t * m + i] = a[start + i];
        if (shape == 4) {
            pos = (int)rnd(start, start + c * m - 1);
            a[pos] = a[pos] % (small ? top + 1 : 100) + 1;
        }
    }
    put_ints(a, n);
    put_int(m);
    put_int(k);
    free(a);
}
