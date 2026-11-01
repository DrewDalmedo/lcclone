/* A power of 7, or one more or one less; a numeral whose digits come
 * mostly from 0, 1 and 6 (so that zeros inside or at the end, and runs of
 * sixes, come up), or any digits; a value near either end of the range;
 * a value near zero; or any value. A quarter to a half of them are made
 * negative. Small cases keep |num| below 7^4 = 2401 (and often tiny);
 * full-size ones use the whole range, up to 9 digits. */
void gen(int small)
{
    static const long long pow7[] = {1, 7, 49, 343, 2401, 16807, 117649,
                                     823543, 5764801};
    long long top = 10000000, v = 0;
    int k, i, len;

    switch ((int)rnd(0, 5)) {
    case 0:
        k = (int)rnd(0, small ? 3 : 8);
        v = pow7[k] + rnd(-1, 1);
        break;
    case 1:
    case 2:
        len = (int)rnd(1, small ? 4 : 9);
        for (i = 0; i < len; i++) {
            int d;

            if (len == 9 && i == 0)
                d = 1; /* 9 digits must stay at most 10^7 */
            else if (len == 9 && i == 1)
                d = (int)rnd(0, 4);
            else if (rnd_chance(60))
                d = rnd_char("016") - '0';
            else
                d = (int)rnd(0, 6);
            v = v * 7 + d;
        }
        break;
    case 3:
        v = small ? rnd(0, 6) : top - rnd(0, 1000);
        break;
    case 4:
        v = small ? rnd(0, 60) : rnd(0, 100000);
        break;
    default:
        v = rnd(0, small ? 2400 : top);
    }
    if (rnd_chance(small ? 50 : 40))
        v = -v;
    put_int(v);
}
