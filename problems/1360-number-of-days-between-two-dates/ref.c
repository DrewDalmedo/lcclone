/* Reference solution: turn each date into its number of days since
 * 1971-01-01 by arithmetic (whole years, with the leap days counted by
 * formula, then whole months, then days), and return the absolute
 * difference. */
static int two(const char *s)
{
    return (s[0] - '0') * 10 + (s[1] - '0');
}

static int leaps_through(int y)
{
    return y / 4 - y / 100 + y / 400;
}

static int day_number(const char *date)
{
    static const int before[12] = {0, 31, 59, 90, 120, 151,
                                   181, 212, 243, 273, 304, 334};
    int y = two(date) * 100 + two(date + 2);
    int m = two(date + 5), d = two(date + 8);
    int leap = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
    int days = (y - 1971) * 365 + leaps_through(y - 1) - leaps_through(1970);

    days += before[m - 1] + (leap && m > 2) + d - 1;
    return days;
}

int daysBetweenDates(char *date1, char *date2)
{
    int diff = day_number(date1) - day_number(date2);

    return diff < 0 ? -diff : diff;
}
