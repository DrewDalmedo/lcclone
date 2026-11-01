/* A power of two (three times in eight), a neighbour of one (one more or
 * one less), a negated power (down to -2^31), an odd number times a power,
 * or any value. Small cases mostly use exponents up to 6 and small values,
 * so 0, 1 and -1 come up often; full-size ones use every exponent up to 30
 * and the whole int range, with 2^30, 2^31 - 1 and -2^31 as the last
 * shape. */
void gen(int small)
{
    int k = (int)rnd(0, small && rnd_chance(70) ? 6 : 30);
    long long n, lim;

    switch ((int)rnd(0, 7)) {
    case 0:
    case 1:
    case 2:
        n = 1LL << k;
        break;
    case 3:
        n = (1LL << k) + (rnd_chance(50) ? 1 : -1);
        break;
    case 4:
        n = -(1LL << (k == 30 && rnd_chance(50) ? 31 : k));
        break;
    case 5: /* (2j + 1) * 2^k with j >= 1, up to 2^31 - 1 */
        if (k == 30)
            k = 29;
        lim = ((2147483647LL >> k) - 1) / 2;
        if (small && lim > 10)
            lim = 10;
        n = (2 * rnd(1, lim) + 1) << k;
        break;
    case 6:
        n = small ? rnd(-20, 70) : rnd(-2147483647LL - 1, 2147483647LL);
        break;
    default:
        if (small)
            n = rnd(-3, 3);
        else {
            static const long long edge[] = {1073741824LL, 2147483647LL,
                                             -2147483647LL - 1};
            n = edge[rnd(0, 2)];
        }
    }
    put_int(n);
}
