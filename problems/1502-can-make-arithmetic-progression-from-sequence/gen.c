/* A progression with a random step, given as is or spoiled in one way:
 * one value nudged, one value copied over another, two inner values each
 * moved one step towards the other (so the smallest, the largest and the
 * sum stay the same), or the largest value moved. Some cases are random
 * values, or all equal. The values come shuffled, ascending or
 * descending. Small cases have at most 8 values in [-10, 10]; full-size
 * ones have 1000 values, spread over as much of [-10^6, 10^6] as the step
 * allows. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 1000;
    int top = small ? 10 : 1000000;
    int dmax = 2 * top / (n - 1), shape = (int)rnd(0, 5), order;
    int *a = malloc((size_t)n * sizeof *a);
    int step, first, i, j, delta, p, q, t;

    if (small && dmax > 3)
        dmax = 3;
    step = (int)rnd(0, dmax);
    if (step == 0 && rnd_chance(60))
        step = 1;
    first = (int)rnd(-top, top - (n - 1) * step);
    for (i = 0; i < n; i++)
        a[i] = first + i * step;
    if (shape == 1) {
        i = (int)rnd(0, n - 1);
        delta = (int)rnd(1, 2);
        if (rnd_chance(50))
            delta = -delta;
        if (a[i] + delta > top || a[i] + delta < -top)
            delta = -delta;
        a[i] += delta;
    } else if (shape == 2) {
        i = (int)rnd(0, n - 1);
        j = (int)rnd(0, n - 1);
        a[i] = a[j];
    } else if (shape == 3 && n >= 5) {
        p = (int)rnd(1, n - 4);
        q = (int)rnd(p + 2, n - 2);
        a[p] += step;
        a[q] -= step;
    } else if (shape == 4) {
        if (a[n - 1] < top)
            a[n - 1]++;
        else
            a[n - 1]--;
    } else if (shape == 5) {
        t = small ? 4 : top;
        if (rnd_chance(30)) {
            first = (int)rnd(-t, t);
            for (i = 0; i < n; i++)
                a[i] = first;
        } else {
            for (i = 0; i < n; i++)
                a[i] = (int)rnd(-t, t);
        }
    }
    order = (int)rnd(0, 5);
    if (order == 1)
        for (i = 0, j = n - 1; i < j; i++, j--) {
            t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    else if (order >= 2)
        rnd_shuffle(a, n);
    put_ints(a, n);
    free(a);
}
