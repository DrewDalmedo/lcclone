/* A grid of 1 to 6 by 1 to 6 cells for small cases. Full-size ones have
 * 10^4 pairs, and each side is 40000 half the time, else past 32767 or of
 * any size. The seed picks the pairs:
 *   - most of the time, the seed picks the corner block A by B that all of
 *     them cover, every pair [a, b] has a >= A and b >= B, and one pair
 *     (sometimes the last one) gets a = A and another one b = B, so that
 *     the two smallest usually come from different pairs;
 *   - now and then every pair is the same;
 *   - or there are none.
 * Small cases have 1 to 8 pairs, and now and then 257 to 300, so that a
 * cell's count passes 255. */
void gen(int small)
{
    int m, n, k, A, B, a, b, i, pa, pb, shape = (int)rnd(0, 9);
    int *ops;

    if (small) {
        m = (int)rnd(1, 6);
        n = (int)rnd(1, 6);
    } else {
        m = rnd_chance(50) ? 40000
            : rnd_chance(50) ? (int)rnd(32768, 40000) : (int)rnd(1, 40000);
        n = rnd_chance(50) ? 40000
            : rnd_chance(50) ? (int)rnd(32768, 40000) : (int)rnd(1, 40000);
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
