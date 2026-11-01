/* Slow and obviously right: lay the digits out in every distinct order and
 * cut the row in every place between two digits; the left part is one
 * number and the right part the other. */
static int next_order(int *d, int n)
{
    int i = n - 2, j, t;

    while (i >= 0 && d[i] >= d[i + 1])
        i--;
    if (i < 0)
        return 0;
    j = n - 1;
    while (d[j] <= d[i])
        j--;
    t = d[i];
    d[i] = d[j];
    d[j] = t;
    for (i++, j = n - 1; i < j; i++, j--) {
        t = d[i];
        d[i] = d[j];
        d[j] = t;
    }
    return 1;
}

int splitNum(int num)
{
    int d[16], n = 0, best = -1, cut, i, j, t, a, b;

    while (num > 0) {
        d[n++] = num % 10;
        num /= 10;
    }
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (d[j] < d[i]) {
                t = d[i];
                d[i] = d[j];
                d[j] = t;
            }
    do {
        for (cut = 1; cut < n; cut++) {
            a = 0;
            b = 0;
            for (i = 0; i < cut; i++)
                a = a * 10 + d[i];
            for (i = cut; i < n; i++)
                b = b * 10 + d[i];
            if (best < 0 || a + b < best)
                best = a + b;
        }
    } while (next_order(d, n));
    return best;
}
