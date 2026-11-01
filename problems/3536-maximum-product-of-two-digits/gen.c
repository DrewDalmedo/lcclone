/* A random n from 10 to 10^9, built digit by digit. The seed picks how the
 * digits are drawn: from 0 to 9; from a narrow band, so the largest digit
 * often repeats; a big leading digit over small ones, so the leading digit
 * is the only big one; or one big digit at a random place among smaller
 * ones. Small cases have 2 to 4 digits; full-size ones mostly 9, and now
 * and then n is 10^9, the only ten-digit input. */
void gen(int small)
{
    int len, shape, i, lo, hi, d, spot, big;
    long long n = 0;

    if (!small && rnd_chance(8)) {
        put_int(1000000000);
        return;
    }
    len = small ? (int)rnd(2, 4) : rnd_chance(70) ? 9 : (int)rnd(2, 8);
    shape = (int)rnd(0, 3);
    hi = (int)rnd(1, 9);
    lo = hi - (int)rnd(0, 3);
    if (lo < 0)
        lo = 0;
    big = (int)rnd(5, 9);
    spot = (int)rnd(0, len - 1);
    for (i = 0; i < len; i++) {
        if (shape == 0)
            d = (int)rnd(0, 9);
        else if (shape == 1)
            d = (int)rnd(lo, hi);
        else if (shape == 2)
            d = i == 0 ? big : (int)rnd(0, big - 5);
        else
            d = i == spot ? big : (int)rnd(0, big - 1);
        if (i == 0 && d == 0)
            d = (int)rnd(1, 9);
        n = n * 10 + d;
    }
    put_int(n);
}
