/* A power of three (three times in eight), a neighbour of one (one more or
 * one less), a negated power (down to -3^19), a power times a number from
 * 2 up that 3 doesn't divide, or any value. Small cases mostly use
 * exponents up to 6 and small values, so 0, 1 and -1 come up often;
 * full-size ones use every exponent up to 19 and the whole int range, and
 * as the last shape an edge: 3^19 or a neighbour, 2^31 - 1 or -2^31. */
void gen(int small)
{
    int k = (int)rnd(0, small && rnd_chance(70) ? 6 : 19), i;
    long long p = 1, n, lim, m;

    for (i = 0; i < k; i++)
        p *= 3;
    switch ((int)rnd(0, 7)) {
    case 0:
    case 1:
    case 2:
        n = p;
        break;
    case 3:
        n = p + (rnd_chance(50) ? 1 : -1);
        break;
    case 4:
        n = -p;
        break;
    case 5: /* m * 3^k with m >= 2 and m % 3 != 0, up to 2^31 - 1 */
        if (k == 19)
            p /= 3;
        lim = 2147483647LL / p;
        if (small && lim > 10)
            lim = 10;
        do
            m = rnd(2, lim);
        while (m % 3 == 0);
        n = m * p;
        break;
    case 6:
        n = small ? rnd(-30, 100) : rnd(-2147483647LL - 1, 2147483647LL);
        break;
    default:
        if (small) {
            n = rnd(-3, 3);
        } else {
            static const long long edge[] = {
                1162261467LL, 1162261466LL, 1162261468LL,
                2147483647LL, -2147483647LL - 1,
            };
            n = edge[rnd(0, 4)];
        }
    }
    put_int(n);
}
