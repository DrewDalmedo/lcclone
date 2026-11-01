/* The seed picks a shape: random values, strictly decreasing (each answer
 * is the next value, and the slowest case for a rescan that starts over
 * when its maximum falls behind), strictly increasing, three neighbouring
 * values only (many ties with the maximum), or random with the largest
 * value placed first, last or anywhere. Small cases have up to 8 values
 * among 1..10 or 99991..10^5; full-size ones have 10^4 values in
 * 1..10^5. */

/* Fills a[0..n) with increasing values from lo to hi, hi - lo + 1 >= n,
 * half the time moved up to end at hi. */
static void ascending(int *a, int n, int lo, int hi)
{
    int g = (hi - lo + 1) / n, v = lo + (int)rnd(0, g - 1), i;

    for (i = 0; i < n; i++) {
        if (i > 0)
            v += (int)rnd(1, g);
        a[i] = v;
    }
    if (rnd_chance(50))
        for (i = 0; i < n; i++)
            a[i] += hi - v;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000;
    int lo = small && rnd_chance(30) ? 99991 : 1;
    int hi = small ? lo + 9 : 100000;
    int shape = (int)rnd(0, 5), i, t, pos;
    int base = rnd_chance(50) ? lo : hi - 2;
    int *arr = malloc((size_t)n * sizeof *arr);

    if (shape == 2 || shape == 3) {
        ascending(arr, n, lo, hi);
        for (i = 0; shape == 2 && i < n - 1 - i; i++) {
            t = arr[i];
            arr[i] = arr[n - 1 - i];
            arr[n - 1 - i] = t;
        }
    } else {
        for (i = 0; i < n; i++)
            arr[i] = shape == 4 ? (int)rnd(base, base + 2) : (int)rnd(lo, hi);
        if (shape == 5) {
            pos = (int)rnd(0, 2);
            pos = pos == 0 ? 0 : pos == 1 ? n - 1 : (int)rnd(0, n - 1);
            arr[pos] = hi;
        }
    }
    put_ints(arr, n);
    free(arr);
}
