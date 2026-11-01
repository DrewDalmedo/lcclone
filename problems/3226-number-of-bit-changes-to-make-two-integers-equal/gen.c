/* Two numbers n and k. The seed picks the shape of n (random, all ones, a
 * power of two, or close to the top of the range) and of k:
 *   a random non-empty subset of n's 1-bits (reachable; 3 cases in 10);
 *   n itself (0 moves; 1 in 10);
 *   a subset of n's 1-bits plus one bit that n lacks, below n's highest
 *   bit when there is one (unreachable, though k is often below n; 2 in 10);
 *   a subset of n's 1-bits plus a bit above n's highest bit (unreachable,
 *   and k's low bits may all be in n; 2 in 10);
 *   any number (2 in 10).
 * Small cases keep both below 64; full-size ones go up to 10^6, so bit 19
 * comes up. */

/* The bits b in [lo, hi] that n lacks and that keep (s | 1 << b) within
 * limit, stored in out; returns how many. */
static int missing(int n, int s, int lo, int hi, int limit, int *out)
{
    int b, cnt = 0;

    for (b = lo; b <= hi && b < 20; b++)
        if (!(n & (1 << b)) && (s | (1 << b)) <= limit)
            out[cnt++] = b;
    return cnt;
}

void gen(int small)
{
    int limit = small ? 63 : 1000000;
    int nshape = (int)rnd(0, 3);
    int kshape = (int)rnd(0, 9);
    int maxones = small ? 6 : 19, maxpow = small ? 5 : 19;
    int n, k, s, m, hi, cnt, bits[20];

    if (nshape == 0) {
        n = (int)rnd(1, limit);
    } else if (nshape == 1) {
        m = (int)rnd(1, maxones);
        n = (1 << m) - 1;
    } else if (nshape == 2) {
        m = (int)rnd(0, maxpow);
        n = 1 << m;
    } else {
        n = (int)rnd(limit - limit / 16, limit);
    }
    for (hi = 19; !(n & (1 << hi)); hi--)
        ;
    s = n & (int)rnd(0, (1 << 20) - 1);
    if (kshape <= 2) {
        k = s != 0 ? s : (n & -n);
    } else if (kshape == 3) {
        k = n;
    } else if (kshape <= 7) {
        if (kshape <= 5) {
            cnt = missing(n, s, 0, hi - 1, limit, bits);
            if (cnt == 0)
                cnt = missing(n, s, hi + 1, 19, limit, bits);
        } else {
            cnt = missing(n, s, hi + 1, 19, limit, bits);
            if (cnt == 0)
                cnt = missing(n, s, 0, hi - 1, limit, bits);
        }
        if (cnt > 0) {
            m = (int)rnd(0, cnt - 1);
            k = s | (1 << bits[m]);
        } else {
            k = (int)rnd(1, limit);
        }
    } else {
        k = (int)rnd(1, limit);
    }
    put_int(n);
    put_int(k);
}
