/* Random arrays. Small ones (up to 8 elements) mostly use tiny values, so
 * equal sums and ties come up often; some are sorted, so nothing is fused.
 * Full-size ones have 50 elements, and the seed picks the shape: random,
 * values from a narrow range, sorted, falling negatives or falling
 * positives (49 fusions or close to it), or values near 1000, whose sums
 * soon pass any bound taken from two inputs. */
static void sort_up(int *a, int n)
{
    int i, j, t;

    for (i = 1; i < n; i++)
        for (j = i; j > 0 && a[j - 1] > a[j]; j--) {
            t = a[j];
            a[j] = a[j - 1];
            a[j - 1] = t;
        }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50, i, gap, low, v;
    int shape = (int)rnd(0, 9);
    int top = !small ? 1000 : shape < 4 ? 3 : shape < 8 ? 10 : 1000;
    int a[50];

    for (i = 0; i < n; i++)
        a[i] = (int)rnd(-top, top);
    if (small) {
        if (shape == 3 || shape == 7)
            sort_up(a, n);
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(-3, 3);
    } else if (shape == 3) {
        sort_up(a, n);
    } else if (shape >= 4 && shape <= 7) {
        /* Strictly falling: steps of gap, each value nudged up by less
         * than gap, from low (at least -1000 or 1) to at most -1 or 1000. */
        gap = (int)rnd(1, 20);
        low = shape <= 5 ? (int)rnd(-1000, -50 * gap)
                         : (int)rnd(1, 1001 - 50 * gap);
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, gap - 1);
            a[i] = low + (49 - i) * gap + v;
        }
    } else if (shape == 8) {
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(950, 1000);
    }
    put_ints(a, n);
}
