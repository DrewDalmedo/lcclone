/* Two readings of one day, the second not earlier than the first. Small
 * cases favour equal readings, short gaps that carry across a minute or
 * an hour, and fields such as 00, 08, 09, 10 and 59; full-size ones pick
 * the shape from the seed: any two readings, or a span of more than 18
 * hours. */
static void put_time(int t)
{
    char buf[16];

    snprintf(buf, sizeof buf, "%02d:%02d:%02d", t / 3600, t / 60 % 60, t % 60);
    put_string(buf);
}

static int edge_time(void)
{
    static const int hours[] = {0, 1, 8, 9, 10, 12, 22, 23};
    static const int sixty[] = {0, 1, 8, 9, 10, 30, 58, 59};
    int h = hours[rnd(0, 7)];
    int m = sixty[rnd(0, 7)];
    int s = sixty[rnd(0, 7)];

    return h * 3600 + m * 60 + s;
}

void gen(int small)
{
    int shape = (int)rnd(0, 3), a, b, gap;

    if (!small) {
        if (shape < 2) {
            a = (int)rnd(0, 86399);
            b = (int)rnd(0, 86399);
        } else {
            a = (int)rnd(0, 19800);
            b = (int)rnd(a + 65536, 86399);
        }
    } else if (shape == 0) {
        a = edge_time();
        b = a;
    } else if (shape == 1) {
        gap = (int)rnd(1, 130);
        a = (int)rnd(0, 86399 - gap);
        if (rnd_chance(50))
            a = a / 60 * 60 + 59;      /* the next second carries */
        if (a > 86399 - gap)
            a = 86399 - gap;
        b = a + gap;
    } else if (shape == 2) {
        a = edge_time();
        b = edge_time();
    } else {
        a = (int)rnd(0, 86399);
        b = (int)rnd(0, 86399);
    }
    if (a > b) {
        gap = a;
        a = b;
        b = gap;
    }
    put_time(a);
    put_time(b);
}
