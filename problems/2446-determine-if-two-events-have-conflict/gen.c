/* Four times drawn from one window of the day, paired up as two events
 * (each pair sorted). The window is usually only a few minutes wide, so
 * shared ends, one-minute gaps, equal and zero-length events come up often;
 * sometimes it is wide or the whole day. Its centre is often a whole hour,
 * so events cross an hour boundary, or is pushed to 00:00 or 23:59. The
 * input is always four times, so full-size cases are made the same way. */
static void put_event(int a, int b)
{
    char buf[2][6];
    char *ev[2];
    int t;

    if (a > b) {
        t = a;
        a = b;
        b = t;
    }
    sprintf(buf[0], "%02d:%02d", a / 60, a % 60);
    sprintf(buf[1], "%02d:%02d", b / 60, b % 60);
    ev[0] = buf[0];
    ev[1] = buf[1];
    put_strings(ev, 2);
}

static int draw(int lo, int hi)
{
    int t = (int)rnd(lo, hi);

    return t < 0 ? 0 : t > 1439 ? 1439 : t;
}

void gen(int small)
{
    int centre, width, kind, a, b, c, d;

    (void)small;
    kind = (int)rnd(0, 9);
    if (kind < 4)
        centre = 60 * (int)rnd(0, 23);
    else if (kind < 5)
        centre = rnd_chance(50) ? 0 : 1439;
    else
        centre = (int)rnd(0, 1439);
    kind = (int)rnd(0, 9);
    if (kind < 6)
        width = (int)rnd(0, 4);
    else if (kind < 9)
        width = (int)rnd(5, 90);
    else
        width = 1440;
    a = draw(centre - width, centre + width);
    b = draw(centre - width, centre + width);
    c = draw(centre - width, centre + width);
    d = draw(centre - width, centre + width);
    put_event(a, b);
    put_event(c, d);
}
