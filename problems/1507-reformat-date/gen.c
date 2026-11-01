/* A real calendar date from 1900 to 2100, written with its ordinal day
 * and month abbreviation. Small cases favour the edges: the first and
 * last years, 2000, days whose suffix or width is special (1st to 3rd,
 * 9th to 13th, 21st to 23rd, the last day of the month) and the 29th of
 * February in leap years. Full-size cases are drawn evenly. */
static int month_days(int year, int month)
{
    static const int days[12] = {31, 28, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
    int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    return days[month - 1] + (month == 2 && leap);
}

static const char *suffix(int day)
{
    if (day == 1 || day == 21 || day == 31)
        return "st";
    if (day == 2 || day == 22)
        return "nd";
    if (day == 3 || day == 23)
        return "rd";
    return "th";
}

void gen(int small)
{
    static const char *names[12] = {"Jan", "Feb", "Mar", "Apr", "May",
                                    "Jun", "Jul", "Aug", "Sep", "Oct",
                                    "Nov", "Dec"};
    static const int years[4] = {1900, 2000, 2100, 2024};
    static const int edges[11] = {1, 2, 3, 9, 10, 11, 12, 13, 21, 22, 23};
    int year = (int)rnd(1900, 2100), month = (int)rnd(1, 12), last, day;
    char s[16];

    if (small && rnd_chance(40))
        year = years[rnd(0, 3)];
    last = month_days(year, month);
    day = (int)rnd(1, last);
    if (small && rnd_chance(60))
        day = rnd_chance(25) ? last : edges[rnd(0, 10)];
    snprintf(s, sizeof s, "%d%s %s %d", day, suffix(day), names[month - 1],
             year);
    put_string(s);
}
