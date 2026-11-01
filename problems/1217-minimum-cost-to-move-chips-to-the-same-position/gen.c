/* Chips at random coordinates. Small cases: 1 to 8 chips at coordinates
 * from 1 to at most 10, so stacks and equal counts of the two parities
 * come up often; sometimes the 10 coordinates just below 10^9 instead.
 * Full size: 100 chips anywhere from 1 to 10^9. The seed picks a shape:
 * any parities; all even; all odd; a lopsided split, either way round; or
 * exactly half of each parity, the largest answer. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 4), pct = 50, i, odd;
    long long lo = 1, hi = small ? rnd(3, 10) : 1000000000, v;
    int *a = malloc((size_t)n * sizeof *a);

    if (small && rnd_chance(20)) {
        lo = 1000000000 - 9;
        hi = 1000000000;
    }
    if (shape == 1)
        pct = 0;
    else if (shape == 2)
        pct = 100;
    else if (shape == 3) {
        pct = (int)rnd(5, 20);
        if (rnd_chance(50))
            pct = 100 - pct;
    }
    for (i = 0; i < n; i++) {
        odd = shape == 4 ? i % 2 : rnd_chance(pct);
        v = rnd(lo, hi);
        if (v % 2 != odd)
            v = v < hi ? v + 1 : v - 1;
        a[i] = (int)v;
    }
    if (shape == 4)
        rnd_shuffle(a, n);
    put_ints(a, n);
    free(a);
}
