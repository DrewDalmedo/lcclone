/* Two dates. Small cases put both near one of the years where the rules
 * bite (the first and last years, the leap years 1972, 2000 and 2096, and
 * 2100, which is not one), favouring January to March and December, so
 * that leap days and year ends lie between them; now and then the dates
 * are equal. Full-size cases take any two dates in range, or the two ends
 * of the range in either order, and so reach the largest answers. */
static int month_length(int y, int m)
{
    static const int len[12] = {31, 28, 31, 30, 31, 30,
                                31, 31, 30, 31, 30, 31};

    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0))
        return 29;
    return len[m - 1];
}

static void put_date(int y, int m, int d)
{
    char s[16];

    sprintf(s, "%04d-%02d-%02d", y, m, d);
    put_string(s);
}

static void random_date(int lo, int hi, int favour_ends, int *y, int *m,
                        int *d)
{
    static const int ends[] = {1, 2, 3, 12};

    *y = (int)rnd(lo, hi);
    if (favour_ends && rnd_chance(70))
        *m = ends[rnd(0, 3)];
    else
        *m = (int)rnd(1, 12);
    if (rnd_chance(30))
        *d = month_length(*y, *m) - (int)rnd(0, 1);
    else
        *d = (int)rnd(1, month_length(*y, *m));
}

void gen(int small)
{
    static const int hot[] = {1971, 1972, 1999, 2000, 2096, 2100};
    int y1, m1, d1, y2, m2, d2, base, lo, hi, shape;

    if (small) {
        base = hot[rnd(0, 5)];
        lo = base - 1 < 1971 ? 1971 : base - 1;
        hi = base + 1 > 2100 ? 2100 : base + 1;
        random_date(lo, hi, 1, &y1, &m1, &d1);
        if (rnd_chance(10)) {
            y2 = y1;
            m2 = m1;
            d2 = d1;
        } else {
            random_date(lo, hi, 1, &y2, &m2, &d2);
        }
    } else {
        shape = (int)rnd(0, 3);
        if (shape == 0) {
            y1 = 1971, m1 = 1, d1 = 1;
            y2 = 2100, m2 = 12, d2 = 31;
        } else if (shape == 1) {
            y1 = 2100, m1 = 12, d1 = 31;
            y2 = 1971, m2 = 1, d2 = 1;
        } else {
            random_date(1971, 2100, 0, &y1, &m1, &d1);
            random_date(1971, 2100, 0, &y2, &m2, &d2);
        }
    }
    put_date(y1, m1, d1);
    put_date(y2, m2, d2);
}
