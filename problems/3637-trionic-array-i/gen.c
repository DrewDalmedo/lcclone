/* nums is built from the directions of its n - 1 steps: each step goes
 * up, down or (rarely) stays level, and its size is random. Small cases
 * have n from 3 to 8 and values in -10..10; full-size ones have n = 100
 * (or any n from 3) and values in -1000..1000, sometimes touching -1000
 * or 1000. The seed picks the directions:
 *   - random values, with no plan at all;
 *   - up, down, up, each part one step or longer (true);
 *   - the same with one step made level: a random one, or one next to a
 *     turning point (false);
 *   - a wrong number of parts, or the wrong first direction (false). */
#define MAXN 100

/* Turns the step directions dir[0..n-1) into values in lo..hi. */
static void build(int *a, int n, const int *dir, int dmax, int lo, int hi,
                  int edge)
{
    int d[MAXN], i, mn, mx, shrink, off;

    for (i = 0; i + 1 < n; i++)
        d[i] = dir[i] * (int)rnd(1, dmax);
    for (;;) {
        a[0] = mn = mx = 0;
        for (i = 1; i < n; i++) {
            a[i] = a[i - 1] + d[i - 1];
            if (a[i] < mn)
                mn = a[i];
            if (a[i] > mx)
                mx = a[i];
        }
        if (mx - mn <= hi - lo)
            break;
        /* Too tall for the range: halve every step, keeping its sign. */
        for (i = 0; i + 1 < n; i++) {
            shrink = d[i] / 2;
            if (shrink == 0 && d[i] != 0)
                shrink = d[i] > 0 ? 1 : -1;
            d[i] = shrink;
        }
    }
    if (edge == 1)
        off = lo - mn;
    else if (edge == 2)
        off = hi - mx;
    else
        off = (int)rnd(lo - mn, hi - mx);
    for (i = 0; i < n; i++)
        a[i] += off;
}

/* Splits the n - 1 steps into parts runs, alternating from sign. */
static void parts(int *dir, int n, int runs, int sign)
{
    int cut[MAXN], i, r, left, len, pos = 0;

    for (r = 0; r < runs; r++) {
        left = (n - 1 - pos) - (runs - 1 - r);
        len = r == runs - 1 ? left : (int)rnd(1, left);
        cut[r] = len;
        pos += len;
    }
    pos = 0;
    for (r = 0; r < runs; r++) {
        for (i = 0; i < cut[r]; i++)
            dir[pos++] = sign;
        sign = -sign;
    }
}

void gen(int small)
{
    int a[MAXN], dir[MAXN], n, shape, lo, hi, dmax, edge, runs, sign, k, i;
    int p;

    n = small ? (int)rnd(3, 8) : (rnd_chance(70) ? MAXN : (int)rnd(3, MAXN));
    lo = small ? -10 : -1000;
    hi = -lo;
    shape = (int)rnd(0, 5);
    if (n < 4 && shape >= 1 && shape <= 3)
        shape = 4;
    if (shape == 0) {
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(small ? -3 : lo, small ? 3 : hi);
        put_ints(a, n);
        return;
    }
    if (shape <= 3) {
        parts(dir, n, 3, 1);
        if (shape == 3) {
            /* Level one step: any, or the one on either side of p. */
            for (p = 0; dir[p] == 1; p++)
                ;
            k = (int)rnd(0, 2);
            if (k == 0)
                k = (int)rnd(0, n - 2);
            else
                k = k == 1 ? p - 1 : p;
            dir[k] = 0;
        }
    } else {
        do {
            runs = (int)rnd(1, n - 1 < 5 ? n - 1 : 5);
            sign = rnd_chance(50) ? 1 : -1;
        } while (runs == 3 && sign == 1);
        parts(dir, n, runs, sign);
    }
    dmax = small ? 3 : (rnd_chance(20) ? 2000 : (int)rnd(1, 40));
    edge = small ? 0 : (int)rnd(0, 3);
    build(a, n, dir, dmax, lo, hi, edge);
    put_ints(a, n);
}
