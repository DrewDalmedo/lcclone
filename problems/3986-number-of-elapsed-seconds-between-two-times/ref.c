/* Reference solution: turn each reading into seconds since midnight,
 * hours * 3600 + minutes * 60 + seconds, and subtract. */
static int seconds_since_midnight(const char *t)
{
    int h = (t[0] - '0') * 10 + (t[1] - '0');
    int m = (t[3] - '0') * 10 + (t[4] - '0');
    int s = (t[6] - '0') * 10 + (t[7] - '0');

    return h * 3600 + m * 60 + s;
}

int secondsBetweenTimes(char *startTime, char *endTime)
{
    return seconds_since_midnight(endTime) - seconds_since_midnight(startTime);
}
