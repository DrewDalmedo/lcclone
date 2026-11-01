/* The seed picks the shape of n: any value, one less than a power of two
 * (only multiples of n + 1 qualify), a power of two (everything below it
 * qualifies), or a value with many 1 bits (often nothing qualifies); and
 * of k: any value, 1 to 3, the largest allowed, or at least 2n + 1, so
 * the window reaches far below 1. Small cases keep n up to 20 and k up to
 * 20 apart from that last shape; full-size ones go up to 100. */
static const int dense[] = { 5, 11, 13, 21, 27, 43, 45, 53, 75, 85, 91, 99 };

static int pick_n(int top)
{
    int shape = (int)rnd(0, 3), m, v;

    if (shape == 1) {
        m = (int)rnd(1, top >= 64 ? 6 : 4);
        v = (1 << m) - 1;
    } else if (shape == 2) {
        m = (int)rnd(0, top >= 64 ? 6 : 4);
        v = 1 << m;
    } else if (shape == 3) {
        v = dense[rnd(0, top >= 99 ? 11 : 2)];
    } else {
        v = (int)rnd(1, top);
    }
    return v;
}

void gen(int small)
{
    int top = small ? 20 : 100;
    int n = pick_n(top);
    int shape = (int)rnd(0, 3), k;

    if (shape == 1)
        k = (int)rnd(1, 3);
    else if (shape == 2)
        k = top;
    else if (shape == 3 && 2 * n + 1 <= 100)
        k = (int)rnd(2 * n + 1, 100);
    else
        k = (int)rnd(1, top);
    put_int(n);
    put_int(k);
}
