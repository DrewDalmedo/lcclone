/* Slow and obviously right: turn every time into minutes since midnight,
 * then walk through all 1440 minutes of the day looking for one that lies
 * inside both events. */
static int minutes(const char *t)
{
    return ((t[0] - '0') * 10 + (t[1] - '0')) * 60 + (t[3] - '0') * 10 + (t[4] - '0');
}

bool haveConflict(char **event1, int event1Size, char **event2, int event2Size)
{
    int s1 = minutes(event1[0]), e1 = minutes(event1[1]);
    int s2 = minutes(event2[0]), e2 = minutes(event2[1]);
    int m;

    (void)event1Size;
    (void)event2Size;
    for (m = 0; m < 24 * 60; m++)
        if (s1 <= m && m <= e1 && s2 <= m && m <= e2)
            return true;
    return false;
}
