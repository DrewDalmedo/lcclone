/* A list of sheets, each with two different side lengths in either order.
 * Small cases have up to 8 sheets with sides from 1..t, where t is 3 to 6,
 * so that ties at the biggest square, equal shorter sides from different
 * pairs and a side equal to the best on a sheet that doesn't reach it come
 * up often.
 * Full-size cases have 1000 sheets, in a shape the seed picks:
 *   0: sides from all of 1..10^9;
 *   1: every sheet has the same shorter side (the answer is 1000);
 *   2: shorter sides rise along the list, so the best is only the last;
 *   3: many ties at a smaller side first, then the best side, near 10^9,
 *      at a few sheets near the end. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int shape = small ? -1 : (int)(gen_seed() % 4);
    int top = small ? (int)rnd(3, 6) : 1000000000;
    int *a = malloc((size_t)n * 2 * sizeof *a);
    int same = (int)rnd(1, 999999999), lo, hi, i, t;

    for (i = 0; i < n; i++) {
        if (shape == 1) {
            lo = same;
            hi = (int)rnd(same + 1, 1000000000);
        } else if (shape == 2) {
            lo = 1000000 * (i + 1) - (int)rnd(1, 999);
            hi = (int)rnd(lo + 1, 1000000000);
        } else if (shape == 3) {
            lo = i < n - 5 ? 500000000 : 999999999;
            if (i >= n - 5 && rnd_chance(50))
                lo = 999999998;
            if (i == n - 1)
                lo = 999999999;
            hi = lo == 999999999 ? 1000000000 : (int)rnd(lo + 1, 1000000000);
        } else {
            lo = (int)rnd(1, top);
            do
                hi = (int)rnd(1, top);
            while (hi == lo);
            if (lo > hi) {
                t = lo;
                lo = hi;
                hi = t;
            }
        }
        if (rnd_chance(50)) {
            a[2 * i] = lo;
            a[2 * i + 1] = hi;
        } else {
            a[2 * i] = hi;
            a[2 * i + 1] = lo;
        }
    }
    put_int_grid(a, n, 2);
    free(a);
}
