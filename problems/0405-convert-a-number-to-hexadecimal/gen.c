/* A number made from hexadecimal digits: a power of 16 or a neighbour of
 * one, digits drawn mostly from 0, 9, a and f (so that inner zeros and the
 * step from 9 to a come up), or any digits; a value near either end of
 * the int range; or any value. Eight digits of 8 or more make a negative
 * number. Small cases lean to short numbers and to numbers near zero;
 * full-size ones to the whole range. */
static long long from_bits(unsigned long long u)
{
    return u >= 2147483648ULL ? (long long)u - 4294967296LL : (long long)u;
}

void gen(int small)
{
    unsigned long long u = 0;
    long long n;
    int k, i, len;

    switch ((int)rnd(0, 5)) {
    case 0: /* 16^k, 16^k - 1 or 16^k + 1, or 16^k - 1 negated */
        k = (int)rnd(0, small ? 4 : 7);
        n = (1LL << (4 * k)) + rnd(-1, 1);
        if (rnd_chance(25))
            n = -n;
        break;
    case 1:
    case 2: /* digits from a small set, or any */
        len = (int)rnd(1, small && rnd_chance(60) ? 4 : 8);
        for (i = 0; i < len; i++) {
            char c = i % 2 == 1 || rnd_chance(50) ? rnd_char("09af")
                                                  : rnd_char("0123456789abcdef");

            u = u * 16 + (unsigned long long)(c <= '9' ? c - '0' : c - 'a' + 10);
        }
        n = from_bits(u);
        break;
    case 3: /* near the ends of the int range */
        k = (int)rnd(0, small ? 3 : 1000);
        n = rnd_chance(50) ? 2147483647LL - k : -2147483647LL - 1 + k;
        break;
    case 4: /* near zero */
        n = small ? rnd(-300, 300) : rnd(-100000, 100000);
        break;
    default:
        n = rnd(-2147483647LL - 1, 2147483647LL);
    }
    put_int(n);
}
