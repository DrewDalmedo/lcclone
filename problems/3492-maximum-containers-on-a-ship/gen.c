/* A random deck size n, container weight w and limit maxWeight. The seed
 * picks maxWeight: anywhere in range; just below, at or just above a
 * multiple of w, near the full deck n * n * w or below it; at most w, so
 * the answer is 0 or 1; or close to 10^9. Small cases keep n up to 4 and
 * w up to 10 (except near 10^9); full-size ones often have n or w at 1000,
 * where a single-precision quotient rounds up. */
void gen(int small)
{
    long long n, w, cells, k, d, mw;
    int shape;

    if (small) {
        n = rnd(1, 4);
        w = rnd(1, 10);
    } else {
        n = rnd_chance(50) ? 1000 : rnd(1, 1000);
        w = rnd_chance(50) ? 1000 : rnd(1, 1000);
    }
    cells = n * n;
    shape = (int)rnd(0, 4);
    d = rnd(-1, 1);
    if (shape == 0) {
        mw = small ? rnd(1, (cells + 2) * w) : rnd(1, 1000000000);
    } else if (shape == 1) {
        k = cells - rnd(-1, 3);
        mw = k * w + d;
    } else if (shape == 2) {
        k = rnd(1, cells);
        mw = k * w + d;
    } else if (shape == 3) {
        mw = rnd(1, w);
    } else {
        mw = 1000000000 - rnd(0, 2000);
    }
    if (mw < 1)
        mw = 1;
    if (mw > 1000000000)
        mw = 1000000000;
    put_int(n);
    put_int(w);
    put_int(mw);
}
