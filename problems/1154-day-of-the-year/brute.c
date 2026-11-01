/* Slow and obviously right: start at January 1 and step through the
 * calendar one day at a time, counting, until the date is reached. */
static int days_in_month(int y, int m)
{
    if (m == 2) {
        if (y % 400 == 0)
            return 29;
        if (y % 100 == 0)
            return 28;
        if (y % 4 == 0)
            return 29;
        return 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11)
        return 30;
    return 31;
}

int dayOfYear(char *date)
{
    int y, m, d, cm = 1, cd = 1, count = 1;

    sscanf(date, "%d-%d-%d", &y, &m, &d);
    while (cm != m || cd != d) {
        count++;
        cd++;
        if (cd > days_in_month(y, cm)) {
            cd = 1;
            cm++;
        }
    }
    return count;
}
