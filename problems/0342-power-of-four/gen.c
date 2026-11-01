/* A power of four (three times in nine); an odd power of two; a power of
 * four plus or minus one; a negated power of two (down to -2^31); three
 * times a power of four, or several distinct powers of four added up; any
 * value; or an edge value. Small cases mostly use 4^0 to 4^3 and small
 * values, so 0, 1 and -1 come up often; full-size ones use every power up
 * to 4^15 = 2^30 and the whole int range, with values around 2^30 and the
 * two ends of the range among the edges. */
void gen(int small)
{
    int top = small && rnd_chance(70) ? 3 : 15, i, bits;
    int j = (int)rnd(0, top);
    long long p = 1LL << (2 * j), n;

    switch ((int)rnd(0, 8)) {
    case 0:
    case 1:
    case 2:
        n = p;
        break;
    case 3: /* 2 * 4^j, up to 2^29 */
        n = 2 * (j == 15 ? p / 4 : p);
        break;
    case 4:
        n = p + (rnd_chance(50) ? 1 : -1);
        break;
    case 5:
        n = -(1LL << rnd(0, small ? 7 : 31));
        break;
    case 6:
        if (rnd_chance(50)) {
            n = 3 * (j == 15 ? p / 4 : p); /* up to 3 * 4^14 */
        } else {
            do {
                n = 0;
                bits = 0;
                for (i = 0; i <= top; i++)
                    if (rnd_chance(50)) {
                        n |= 1LL << (2 * i);
                        bits++;
                    }
            } while (bits < 2);
        }
        break;
    case 7:
        n = small ? rnd(-20, 70) : rnd(-2147483647LL - 1, 2147483647LL);
        break;
    default:
        if (small) {
            n = rnd(-3, 3);
        } else {
            static const long long edge[] = {
                1073741824LL, 1073741823LL, 1073741825LL, 1431655765LL,
                2147483647LL, -2147483647LL - 1
            };

            n = edge[rnd(0, 5)];
        }
    }
    put_int(n);
}
