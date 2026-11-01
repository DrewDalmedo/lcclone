/* A random valid date from 1900 to 2100. The ends of the range and the
 * years where the year gains a binary digit (2047, 2048) come up often,
 * and so do the first and the last day of a month. Every date is short,
 * so small and full-size cases are made alike. */
static int days_in(int year, int month)
{
    static const int days[12] = {31, 28, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
    int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    return month == 2 && leap ? 29 : days[month - 1];
}

void gen(int small)
{
    static const int special[] = {1900, 1999, 2000, 2047, 2048, 2100};
    char s[16];
    int year, month, day, last;

    (void)small;
    if (rnd_chance(30))
        year = special[rnd(0, 5)];
    else
        year = (int)rnd(1900, 2100);
    month = (int)rnd(1, 12);
    last = days_in(year, month);
    if (rnd_chance(25))
        day = rnd_chance(50) ? 1 : last;
    else
        day = (int)rnd(1, last);
    sprintf(s, "%04d-%02d-%02d", year, month, day);
    put_string(s);
}
