/* Random ball counts. The seed picks a shape: any two counts; counts on
 * or one below the totals that odd rows (perfect squares) and even rows
 * (m * (m + 1)) need; one color with only 1 or 2 balls; equal or nearly
 * equal counts; or counts near the largest allowed. Then the two colors
 * may swap. Small cases stay at 30 balls or fewer. */
void gen(int small)
{
    int top = small ? 30 : 100;
    int shape = (int)rnd(0, 4);
    int a, b, m, d, t;

    if (shape == 0) {
        a = (int)rnd(1, top);
        b = (int)rnd(1, top);
    } else if (shape == 1) {
        m = (int)rnd(1, small ? 5 : 10);
        d = (int)rnd(0, 1);
        a = m * m - d;
        if (a < 1)
            a = 1;
        m = (int)rnd(1, small ? 5 : 9);
        d = (int)rnd(0, 1);
        b = m * (m + 1) - d;
    } else if (shape == 2) {
        a = (int)rnd(1, 2);
        b = (int)rnd(1, top);
    } else if (shape == 3) {
        a = (int)rnd(1, top);
        d = (int)rnd(-2, 2);
        b = a + d;
        if (b < 1)
            b = 1;
        if (b > top)
            b = top;
    } else {
        a = (int)rnd(top - 15, top);
        b = (int)rnd(top - 15, top);
    }
    if (rnd_chance(50)) {
        t = a;
        a = b;
        b = t;
    }
    put_int(a);
    put_int(b);
}
