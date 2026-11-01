/* Two stays in a non-leap year, given to Alice and Bob either way round.
 * One stay is random; the other, picked by the seed, is random too, or
 * starts on the last day of the first one or the day after, or ends on
 * its first day or the day before, or equals it, or lies inside it. Small
 * cases put most random dates within a few days of the end of a month
 * (February's 28th included), and now and then on January 1; full-size
 * cases draw them evenly from the whole year. */
static const int length[12] = {31, 28, 31, 30, 31, 30,
                               31, 31, 30, 31, 30, 31};

static int pick(int small)
{
    int month, end, off, d, i;

    if (!small || rnd_chance(30))
        return (int)rnd(1, 365);
    if (rnd_chance(10))
        return 1;
    month = (int)rnd(0, 11);
    end = 0;
    for (i = 0; i <= month; i++)
        end += length[i];       /* the last day of that month */
    off = (int)rnd(-2, 3);
    d = end + off;
    return d < 1 ? 1 : d > 365 ? 365 : d;
}

static void put_date(int d)
{
    char s[24];
    int month = 0;

    while (d > length[month]) {
        d -= length[month];
        month++;
    }
    snprintf(s, sizeof s, "%02d-%02d", month + 1, d);
    put_string(s);
}

void gen(int small)
{
    int a1, a2, b1, b2, tmp, shape;

    a1 = pick(small);
    a2 = pick(small);
    if (a1 > a2) {
        tmp = a1;
        a1 = a2;
        a2 = tmp;
    }
    shape = (int)rnd(0, 5);
    if (shape == 0) {
        b1 = pick(small);
        b2 = pick(small);
        if (b1 > b2) {
            tmp = b1;
            b1 = b2;
            b2 = tmp;
        }
    } else if (shape == 1) {        /* starts on or right after a2 */
        b1 = a2 + (int)rnd(0, 1);
        if (b1 > 365)
            b1 = 365;
        b2 = b1 + (int)rnd(0, 365 - b1);
    } else if (shape == 2) {        /* ends on or right before a1 */
        b2 = a1 - (int)rnd(0, 1);
        if (b2 < 1)
            b2 = 1;
        b1 = (int)rnd(1, b2);
    } else if (shape == 3) {
        b1 = a1;
        b2 = a2;
    } else {                        /* inside the first stay */
        b1 = (int)rnd(a1, a2);
        b2 = (int)rnd(b1, a2);
    }
    if (rnd_chance(50)) {
        put_date(a1);
        put_date(a2);
        put_date(b1);
        put_date(b2);
    } else {
        put_date(b1);
        put_date(b2);
        put_date(a1);
        put_date(a2);
    }
}
