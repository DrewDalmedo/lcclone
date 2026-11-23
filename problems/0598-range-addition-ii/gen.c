/* A grid of 1 to 6 by 1 to 6 cells for small cases, with pairs of these
 * kinds, drawn:
 *   - most of the time, a corner block A by B that all of them cover:
 *     every pair [a, b] has a >= A and b >= B, and one pair (sometimes the
 *     last one) gets a = A and another one b = B, so that the two smallest
 *     usually come from different pairs;
 *   - now and then every pair is the same;
 *   - or there are none.
 * Small cases have 1 to 8 pairs, and now and then 257 to 300, so that a
 * cell's count passes 255. Full-size ones have 10^4 pairs on a grid of
 * 40000 by 40000, and the seed picks: no pairs (the largest answer, with
 * no pair to shrink it); every pair the whole grid (the same answer); a
 * corner block; or a corner block on sides past 32767. */
void gen(int small)
{
    int m, n, k, A, B, a, b, i, pa, pb, shape = (int)rnd(0, 9);
    int full = small ? -1 : (int)(gen_seed() % 4);
    int *ops;

    if (small) {
        m = (int)rnd(1, 6);
        n = (int)rnd(1, 6);
    } else {
        m = full == 3 ? (int)rnd(32768, 39999) : 40000;
        n = full == 3 ? (int)rnd(32768, 39999) : 40000;
        shape = full == 0 ? 0 : full == 1 ? 2 : 3;
    }
    if (shape == 0)
        k = 0;
    else if (!small)
        k = 10000;
    else if (shape == 1)
        k = (int)rnd(257, 300);
    else
        k = (int)rnd(1, 8);
    ops = malloc(((size_t)2 * k + 1) * sizeof *ops);
    A = (int)rnd(1, m);
    B = (int)rnd(1, n);
    if (full == 1) {
        A = m;
        B = n;
    }
    if (shape == 2) {
        for (i = 0; i < k; i++) {
            ops[2 * i] = A;
            ops[2 * i + 1] = B;
        }
    } else if (k > 0) {
        for (i = 0; i < k; i++) {
            a = (int)rnd(A, m);
            b = (int)rnd(B, n);
            ops[2 * i] = a;
            ops[2 * i + 1] = b;
        }
        pa = rnd_chance(25) ? k - 1 : (int)rnd(0, k - 1);
        pb = rnd_chance(25) ? k - 1 : (int)rnd(0, k - 1);
        ops[2 * pa] = A;
        ops[2 * pb + 1] = B;
    }
    put_int(m);
    put_int(n);
    put_int_grid(ops, k, 2);
    free(ops);
}
