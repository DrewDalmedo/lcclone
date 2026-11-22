/* One int. Small cases have up to five digits, and half of them are
 * palindromes, some negative. Full-size ones reach the ends of the int
 * range, in a shape the seed picks: a palindrome of ten digits (the most
 * an int has), one of those with a single digit changed, a number whose
 * digits reversed don't fit in an int, a negative number at the bottom of
 * the range, or any int. */

/* The palindrome whose first half is `half`, with `digits` digits in all. */
static long long mirror(long long half, int digits)
{
    long long p = half, rest = digits % 2 ? half / 10 : half;

    while (rest > 0) {
        p = p * 10 + rest % 10;
        rest /= 10;
    }
    return p;
}

static long long ten_to(int k)
{
    long long p = 1;

    while (k-- > 0)
        p *= 10;
    return p;
}

void gen(int small)
{
    long long x, p, place, digit;
    int digits, k;

    if (small) {
        digits = (int)rnd(1, 5);
        if (rnd_chance(50))
            x = mirror(rnd(ten_to((digits - 1) / 2),
                           ten_to((digits + 1) / 2) - 1), digits);
        else
            x = rnd(digits == 1 ? 0 : ten_to(digits - 1),
                    ten_to(digits) - 1);
        if (rnd_chance(20))
            x = -x;
        put_int(x);
        return;
    }
    switch (gen_seed() % 5) {
    case 0: /* a palindrome of ten digits: 2147447412 is the largest int one */
        x = mirror(rnd(10000, 21474), 10);
        break;
    case 1: /* the same with one digit changed, so it is no palindrome */
        p = mirror(rnd(10000, 21474), 10);
        do {
            k = (int)rnd(0, 9);
            place = ten_to(k);
            digit = rnd(k == 9 ? 1 : 0, 9);
            x = p - (p / place % 10) * place + digit * place;
        } while (x > 2147483647LL || x == mirror(x / 100000, 10));
        break;
    case 2: /* ten digits ending in 3 to 9: reversed, they pass 2^31 - 1 */
        x = rnd(100000000, 214748363) * 10;
        x += rnd(3, 9);
        break;
    case 3: /* negative, at or near the bottom of the range */
        x = rnd_chance(50) ? -2147483648LL : rnd(-2147483648LL, -2000000000);
        break;
    default:
        x = rnd(-2147483648LL, 2147483647LL);
        break;
    }
    put_int(x);
}
