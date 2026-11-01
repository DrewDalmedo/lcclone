/* One valid date from 1 January 1971 to 31 December 2100. Years that
 * test the leap rules or end the range (1971, 1972, 1999, 2000, 2096,
 * 2099, 2100), February, and the first and last days of a month come up
 * more often than chance would have them. Every date is quick to work
 * out, so `small` changes nothing. */
void gen(int small)
{
    static const int edge_years[7] = {1971, 1972, 1999, 2000, 2096, 2099, 2100};
    static const int len[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year, month, last, day;

    (void)small;
    if (rnd_chance(40))
        year = edge_years[rnd(0, 6)];
    else
        year = (int)rnd(1971, 2100);
    if (rnd_chance(30))
        month = 2;
    else
        month = (int)rnd(1, 12);
    last = len[month - 1];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        last = 29;
    if (rnd_chance(30))
        day = last;
    else if (rnd_chance(15))
        day = 1;
    else
        day = (int)rnd(1, last);
    put_int(day);
    put_int(month);
    put_int(year);
}
