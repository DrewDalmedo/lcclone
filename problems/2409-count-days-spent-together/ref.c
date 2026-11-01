/* Reference solution: turn each date into its day of the year (the days
 * of the earlier months plus the day of the month); the common days run
 * from the later arrival to the earlier departure, if that range is not
 * empty. */
static int day_of_year(const char *date)
{
    static const int before[12] = {0, 31, 59, 90, 120, 151,
                                   181, 212, 243, 273, 304, 334};
    int month = (date[0] - '0') * 10 + (date[1] - '0');
    int day = (date[3] - '0') * 10 + (date[4] - '0');

    return before[month - 1] + day;
}

int countDaysTogether(char *arriveAlice, char *leaveAlice, char *arriveBob,
                      char *leaveBob)
{
    int a1 = day_of_year(arriveAlice), a2 = day_of_year(leaveAlice);
    int b1 = day_of_year(arriveBob), b2 = day_of_year(leaveBob);
    int start = a1 > b1 ? a1 : b1, end = a2 < b2 ? a2 : b2;

    return end >= start ? end - start + 1 : 0;
}
