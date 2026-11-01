/* Small cases have 1 to 12 numbers, mostly 1 to 9 (some above 65535), and
 * a shape the seed picks: one that fits (n a divisor of the length); a
 * near miss with m = length / n rounded down; any m and n up to 6; or a
 * huge shape, 4 * 10^4 rows or columns. Full-size cases, also by seed:
 * 5 * 10^4 numbers split by a random divisor; 4 * 10^4 rows of one; one
 * row of 4 * 10^4; 4 * 10^4 by 4 * 10^4 for 5 * 10^4 numbers (no fit);
 * or a near miss with a random length. */
#define MAXMN 40000

/* A random divisor d of len with len / d and d both at most MAXMN. */
static int divisor(int len)
{
    int ds[64], k = 0, d;

    for (d = 1; d <= len && d <= MAXMN; d++)
        if (len % d == 0 && len / d <= MAXMN && k < 64)
            ds[k++] = d;
    return ds[rnd(0, k - 1)];
}

void gen(int small)
{
    int len, m, n, shape, i, *a;

    if (small) {
        len = (int)rnd(1, 12);
        shape = (int)rnd(0, 7);
        if (shape <= 2) {
            n = divisor(len);
            m = len / n;
        } else if (shape <= 4) {
            n = (int)rnd(1, len + 1);
            m = len / n > 0 ? len / n : 1;
        } else if (shape == 5 || shape == 6) {
            m = (int)rnd(1, 6);
            n = (int)rnd(1, 6);
        } else {
            m = rnd_chance(50) ? MAXMN : (int)rnd(1, 3);
            n = m == MAXMN && rnd_chance(50) ? (int)rnd(1, 3) : MAXMN;
        }
    } else {
        shape = (int)(gen_seed() % 5);
        if (shape == 0) {
            len = 50000;
            n = divisor(len);
            m = len / n;
        } else if (shape == 1 || shape == 2) {
            len = MAXMN;
            m = shape == 1 ? MAXMN : 1;
            n = shape == 1 ? 1 : MAXMN;
        } else if (shape == 3) {
            len = 50000;
            m = n = MAXMN;
        } else {
            len = (int)rnd(1, 50000);
            n = (int)rnd(2, 1000);
            m = len / n > 0 ? len / n : 1;
        }
    }
    a = malloc((size_t)len * sizeof *a);
    for (i = 0; i < len; i++)
        if (!small)
            a[i] = (int)rnd(1, 100000);
        else if (rnd_chance(15))
            a[i] = (int)rnd(65536, 100000);
        else
            a[i] = (int)rnd(1, 9);
    put_ints(a, len);
    put_int(m);
    put_int(n);
    free(a);
}
