/* The seed picks the shape: any value, a multiple of 9, a value near the
 * top (within 10 of 2^31 - 1 in full-size cases, where num + 8 overflows),
 * a number heavy in 9s (large digit sums, so more rounds), or a single
 * digit (full size: any 10-digit value). Small cases stay below 10^4, so
 * 0, single digits and multiples of 9 come up often. */
void gen(int small)
{
    long long num;
    int len, i;

    switch ((int)rnd(0, 4)) {
    case 0:
        num = rnd(0, small ? 9999 : 2147483647);
        break;
    case 1:
        num = 9 * rnd(0, small ? 1111 : 238609294);
        break;
    case 2:
        num = small ? rnd(9990, 9999) : 2147483647 - rnd(0, 10);
        break;
    case 3:
        /* Small: up to 4 digits, each a 9 half the time. Full size: 9
         * digits, each a 9 four times in five, after a 1 half the time. */
        num = small ? 0 : rnd(0, 1);
        len = small ? (int)rnd(1, 4) : 9;
        for (i = 0; i < len; i++)
            num = num * 10 + (rnd_chance(small ? 50 : 80) ? 9 : rnd(0, 9));
        break;
    default:
        num = small ? rnd(0, 9) : rnd(1000000000, 2147483647);
    }
    put_int(num);
}
