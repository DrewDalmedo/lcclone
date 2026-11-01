/* A matrix and a tick count k. Small cases have 1 to 4 rows of 1 to 8
 * values (4, 6 or 8 values half the time, as those n have divisors other
 * than 1 and n); full-size ones are 25 by 25 most of the time, else of a random
 * size. The seed picks k:
 *   0-1: any k from 1 to 12 in small cases, from 1 to 50 in full-size ones;
 *   2:   a multiple of n, so every row comes back;
 *   3:   n + 1, which does what one tick does;
 *   4:   45 to 50 in small cases, 49 in full-size ones (with n = 25 the
 *        rows must then be constant, and nothing ends the check early);
 *   5-7: a multiple of a divisor of n other than 1 and n, when n has one,
 *        but not a multiple of n, so that gcd(k, n) is neither 1 nor n.
 * Two times in three every row repeats with a period that divides
 * gcd(k, n), so the answer would be true; then, 4 times in 10, one value
 * of one row (often the last) is changed, which usually makes it false.
 * Otherwise the values are random. Values come from 1 to 2 or 1 to 3
 * most of the time, so rows repeat by chance too, else from 1 to 25, or
 * are all 1 one time in ten. */
static int gcd(int a, int b)
{
    int t;

    while (b) {
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

void gen(int small)
{
    int m, n, k, g, p, q, top, shape, periodic, i, j, r, c, v, d, ndiv = 0;
    int base[25], divs[25];
    int *mat;

    if (small) {
        m = (int)rnd(1, 4);
        if (rnd_chance(50))
            n = (int)rnd(1, 8);
        else
            n = 4 + 2 * (int)rnd(0, 2);
    } else if (rnd_chance(60)) {
        m = 25;
        n = 25;
    } else {
        m = (int)rnd(1, 25);
        n = (int)rnd(1, 25);
    }
    for (d = 2; d < n; d++)
        if (n % d == 0)
            divs[ndiv++] = d;
    shape = (int)rnd(0, 7);
    if (shape == 2) {
        k = n * (int)rnd(1, 50 / n);
    } else if (shape == 3) {
        k = n + 1;
    } else if (shape == 4) {
        k = small ? (int)rnd(45, 50) : 49;
    } else if (shape >= 5 && ndiv > 0) {
        d = divs[rnd(0, ndiv - 1)];
        q = (int)rnd(1, 50 / d);
        if (d * q % n == 0)
            q = q > 1 ? q - 1 : q + 1;
        k = d * q;
    } else {
        k = (int)rnd(1, small ? 12 : 50);
    }
    g = gcd(k, n);
    v = (int)rnd(0, 19);
    top = v < 2 ? 1 : v < 9 ? 2 : v < 15 ? 3 : 25;
    periodic = rnd_chance(67);
    p = g;
    if (periodic && rnd_chance(25)) {
        q = (int)rnd(1, g);
        if (g % q == 0)
            p = q;
    }
    mat = malloc((size_t)m * n * sizeof *mat);
    for (i = 0; i < m; i++) {
        if (periodic) {
            for (j = 0; j < p; j++)
                base[j] = (int)rnd(1, top);
            for (j = 0; j < n; j++)
                mat[i * n + j] = base[j % p];
        } else {
            for (j = 0; j < n; j++)
                mat[i * n + j] = (int)rnd(1, top);
        }
    }
    if (periodic && rnd_chance(40)) {
        if (rnd_chance(50))
            r = m - 1;
        else
            r = (int)rnd(0, m - 1);
        c = (int)rnd(0, n - 1);
        v = (int)rnd(1, top < 2 ? 2 : top);
        if (v == mat[r * n + c])
            v = v % 25 + 1;
        mat[r * n + c] = v;
    }
    put_int_grid(mat, m, n);
    put_int(k);
    free(mat);
}
