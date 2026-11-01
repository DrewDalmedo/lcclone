/* A number built digit by digit, with zeros coming up often (in the middle,
 * in runs and at the end). Small cases have one to four digits. Full-size
 * ones have up to nine, and the seed picks the shape: random digits, nine
 * digits with no zero (the longest answers), a single non-zero digit
 * followed by zeros, or 10^9 itself. */
void gen(int small)
{
    int len, zero_pct, shape = small ? 0 : (int)rnd(0, 5), i, d;
    long long n;

    if (shape == 4) {
        put_int(1000000000);
        return;
    }
    len = small ? (int)rnd(1, 4) : (int)rnd(1, 9);
    if (shape == 2)
        len = 9;
    zero_pct = shape == 2 ? 0 : shape == 3 ? 100 : (int)rnd(20, 70);
    n = (int)rnd(1, 9);
    for (i = 1; i < len; i++) {
        d = rnd_chance(zero_pct) ? 0 : (int)rnd(1, 9);
        n = n * 10 + d;
    }
    put_int(n);
}
