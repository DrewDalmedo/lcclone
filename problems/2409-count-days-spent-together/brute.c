/* Slow and obviously right: go through every day of the year, write it as
 * "MM-DD", and count the days that lie in both stays. Zero-padded dates in
 * the same year sort as strings in calendar order, so strcmp compares
 * them. */
int countDaysTogether(char *arriveAlice, char *leaveAlice, char *arriveBob,
                      char *leaveBob)
{
    static const int length[12] = {31, 28, 31, 30, 31, 30,
                                   31, 31, 30, 31, 30, 31};
    char date[24];
    int month, day, together = 0;

    for (month = 1; month <= 12; month++)
        for (day = 1; day <= length[month - 1]; day++) {
            snprintf(date, sizeof date, "%02d-%02d", month, day);
            if (strcmp(arriveAlice, date) <= 0 && strcmp(date, leaveAlice) <= 0
                && strcmp(arriveBob, date) <= 0 && strcmp(date, leaveBob) <= 0)
                together++;
        }
    return together;
}
