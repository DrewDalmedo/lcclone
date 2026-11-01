/* Slow and obviously right: copy the start reading and move it on one
 * second at a time, carrying digit by digit like a real clock, until it
 * shows the end reading. */
static void tick(char *t)
{
    if (++t[7] <= '9')
        return;
    t[7] = '0';
    if (++t[6] <= '5')
        return;
    t[6] = '0';
    if (++t[4] <= '9')
        return;
    t[4] = '0';
    if (++t[3] <= '5')
        return;
    t[3] = '0';
    if (++t[1] > '9') {
        t[1] = '0';
        t[0]++;
    }
    if (t[0] == '2' && t[1] == '4') {
        t[0] = '0';
        t[1] = '0';
    }
}

int secondsBetweenTimes(char *startTime, char *endTime)
{
    char now[9];
    int count = 0;

    memcpy(now, startTime, 8);
    now[8] = '\0';
    while (memcmp(now, endTime, 8) != 0 && count < 86400) {
        tick(now);
        count++;
    }
    return count;
}
