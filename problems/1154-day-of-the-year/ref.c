/* Reference solution: read the year, month and day from their fixed
 * places, then add the day to the number of days in the months before it
 * (a table of running totals for a common year), plus one from March on in
 * a leap year. */
int dayOfYear(char *date)
{
    static const int before[12] = {0,   31,  59,  90,  120, 151,
                                   181, 212, 243, 273, 304, 334};
    int y = (date[0] - '0') * 1000 + (date[1] - '0') * 100 +
            (date[2] - '0') * 10 + (date[3] - '0');
    int m = (date[5] - '0') * 10 + (date[6] - '0');
    int d = (date[8] - '0') * 10 + (date[9] - '0');
    int leap = (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;

    return before[m - 1] + d + (leap && m > 2);
}
