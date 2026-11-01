/* One date from 1900-01-01 to 2019-12-31. Small cases lean on the years
 * that trip up a leap rule (1900, 2000 and ordinary leap years), on
 * February and March, and on the first and last day of a month; full-size
 * ones are any date in the range. */
static int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

void gen(int small)
{
    static const int len[12] = {31, 28, 31, 30, 31, 30,
                                31, 31, 30, 31, 30, 31};
    static const int odd[6] = {1900, 2000, 1904, 1996, 2016, 2019};
    char s[16];
    int y, m, d, days, pick;

    if (small && rnd_chance(50))
        y = odd[rnd(0, 5)];
    else
        y = (int)rnd(1900, 2019);
    if (small && rnd_chance(30))
        m = (int)rnd(2, 3);
    else
        m = (int)rnd(1, 12);
    days = len[m - 1] + (m == 2 && is_leap(y));
    pick = small ? (int)rnd(0, 3) : 3;
    if (pick == 0)
        d = 1;
    else if (pick == 1)
        d = days;
    else
        d = (int)rnd(1, days);
    snprintf(s, sizeof s, "%04d-%02d-%02d", y, m, d);
    put_string(s);
}
