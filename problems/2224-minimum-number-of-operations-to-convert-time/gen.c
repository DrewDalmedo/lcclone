/* Two valid times, the second not earlier than the first. Small cases mostly
 * keep the gap under two hours, so that equal times, gaps of a few minutes
 * and minute borrows come up often; full-size ones spread the gap over the
 * whole day, and some run from 00:00 to 23:59 or start late in the day. */
static void put_time(int t)
{
    char s[6];

    s[0] = (char)('0' + t / 60 / 10);
    s[1] = (char)('0' + t / 60 % 10);
    s[2] = ':';
    s[3] = (char)('0' + t % 60 / 10);
    s[4] = (char)('0' + t % 60 % 10);
    s[5] = '\0';
    put_string(s);
}

void gen(int small)
{
    int shape = (int)rnd(0, 3), from, to;

    if (!small && shape == 0) {
        from = (int)rnd(0, 3);
        to = (int)rnd(1436, 1439);
    } else {
        from = (int)rnd(0, 1439);
        if (small && shape != 3)
            to = from + (int)rnd(0, 120);
        else
            to = (int)rnd(from, 1439);
        if (to > 1439)
            to = 1439;
    }
    put_time(from);
    put_time(to);
}
