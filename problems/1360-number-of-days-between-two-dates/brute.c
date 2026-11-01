/* Slow and obviously right: start at the earlier date and move one day
 * at a time, rolling the day over at the end of a month and the month
 * over at the end of a year, until the later date is reached. */
static int month_length(int y, int m)
{
    if (m == 2) {
        if (y % 400 == 0)
            return 29;
        if (y % 100 == 0)
            return 28;
        return y % 4 == 0 ? 29 : 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11)
        return 30;
    return 31;
}

int daysBetweenDates(char *date1, char *date2)
{
    int y1, m1, d1, y2, m2, d2, steps = 0;

    if (strcmp(date1, date2) > 0) {
        char *t = date1;

        date1 = date2;
        date2 = t;
    }
    sscanf(date1, "%d-%d-%d", &y1, &m1, &d1);
    sscanf(date2, "%d-%d-%d", &y2, &m2, &d2);
    while (y1 != y2 || m1 != m2 || d1 != d2) {
        d1++;
        if (d1 > month_length(y1, m1)) {
            d1 = 1;
            m1++;
            if (m1 > 12) {
                m1 = 1;
                y1++;
            }
        }
        steps++;
    }
    return steps;
}
