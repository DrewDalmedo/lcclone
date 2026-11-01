/* Reference solution: turn both times into minutes since midnight. Each
 * step size divides the next (1, 5, 15, 60), so taking as many of the
 * largest step as fit, then the next, and so on, is optimal. */
static int minutes(const char *t)
{
    return ((t[0] - '0') * 10 + (t[1] - '0')) * 60
           + (t[3] - '0') * 10 + (t[4] - '0');
}

int convertTime(char *current, char *correct)
{
    static const int step[4] = {60, 15, 5, 1};
    int d = minutes(correct) - minutes(current), presses = 0, i;

    for (i = 0; i < 4; i++) {
        presses += d / step[i];
        d %= step[i];
    }
    return presses;
}
