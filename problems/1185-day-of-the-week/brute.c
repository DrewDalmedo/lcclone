/* Slow and obviously right: start at 1 January 1971, a Friday, and step
 * through the calendar one day at a time until the date, moving on to
 * the next weekday at each step. */
static int month_length(int month, int year)
{
    static const int len[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

    return month == 2 && leap ? 29 : len[month - 1];
}

char *dayOfTheWeek(int day, int month, int year)
{
    static const char *const names[7] = {
        "Friday", "Saturday", "Sunday", "Monday",
        "Tuesday", "Wednesday", "Thursday"
    };
    int d = 1, m = 1, y = 1971, weekday = 0;
    char *out;

    while (d != day || m != month || y != year) {
        weekday = (weekday + 1) % 7;
        d++;
        if (d > month_length(m, y)) {
            d = 1;
            m++;
            if (m > 12) {
                m = 1;
                y++;
            }
        }
    }
    out = malloc(strlen(names[weekday]) + 1);
    strcpy(out, names[weekday]);
    return out;
}
