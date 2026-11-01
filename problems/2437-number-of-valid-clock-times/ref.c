/* Reference solution: the hour and the minute can be chosen separately, so
 * the answer is (hours that fit the first two places) times (minutes that
 * fit the last two). Each count follows from which places are '?'. */
int countTime(char *time)
{
    int hours, minutes;

    if (time[0] == '?' && time[1] == '?')
        hours = 24;
    else if (time[0] == '?')
        hours = time[1] <= '3' ? 3 : 2; /* x0..x3 allow 0, 1, 2 in front */
    else if (time[1] == '?')
        hours = time[0] == '2' ? 4 : 10; /* 20..23, or 00..09 / 10..19 */
    else
        hours = 1;

    if (time[3] == '?' && time[4] == '?')
        minutes = 60;
    else if (time[3] == '?')
        minutes = 6; /* tens digit 0..5 */
    else if (time[4] == '?')
        minutes = 10;
    else
        minutes = 1;

    return hours * minutes;
}
