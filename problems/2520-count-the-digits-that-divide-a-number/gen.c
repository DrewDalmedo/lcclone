/* One number with no digit 0. Small cases have 1 to 5 digits, full-size
 * ones 9. Half the cases are a multiple of a number with many small
 * factors (such as 24 or 72), so that most of the digits get a tick; the
 * others draw every digit from a small set (such as 2, 4 and 8, or 3 and
 * 6), so that digits repeat. */
static int has_zero(long long v)
{
    for (; v > 0; v /= 10)
        if (v % 10 == 0)
            return 1;
    return 0;
}

void gen(int small)
{
    static const char *sets[] = {"123456789", "248", "36", "1379", "5",
                                 "126", "23"};
    static const int mults[] = {2, 3, 4, 6, 8, 9, 12, 24, 36, 72};
    int digits = small ? (int)rnd(1, 5) : 9, i, k;
    long long lo = 1, hi, num = 0, m;

    for (i = 1; i < digits; i++)
        lo *= 10;
    hi = lo * 10 - 1;
    k = (int)rnd(0, 9);
    m = mults[k];
    if (rnd_chance(50) && hi / m >= (lo + m - 1) / m) {
        do
            num = rnd((lo + m - 1) / m, hi / m) * m;
        while (has_zero(num));
    } else {
        k = (int)rnd(0, 6);
        for (i = 0; i < digits; i++)
            num = num * 10 + (rnd_char(sets[k]) - '0');
    }
    put_int(num);
}
