/* Three counts, up to a small cap (1 to 8) in small cases and up to 100 in
 * full-size ones. The seed picks the shape: three random counts; two counts
 * and a third within 1 of their sum, so that both sides of the boundary
 * a + b = c come up; one count at least as large as the other two together;
 * or three equal counts, one of them sometimes moved by 1. The three are
 * then put in a random order. */
void gen(int small)
{
    int top = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 3);
    int a[3], x, y, c, d, v, i;

    if (shape == 0) {
        for (i = 0; i < 3; i++)
            a[i] = (int)rnd(0, top);
    } else if (shape == 1) {
        x = (int)rnd(0, top / 2);
        y = (int)rnd(0, top - x);
        d = (int)rnd(-1, 1);
        c = x + y + d;
        if (c > top)
            c = top;
        if (c < 0)
            c = 0;
        a[0] = x;
        a[1] = y;
        a[2] = c;
    } else if (shape == 2) {
        c = (int)rnd(top / 2, top);
        x = (int)rnd(0, c / 2);
        y = (int)rnd(0, c - x);
        a[0] = x;
        a[1] = y;
        a[2] = c;
    } else {
        v = (int)rnd(0, top);
        a[0] = a[1] = a[2] = v;
        d = (int)rnd(-1, 1);
        if (v + d >= 0 && v + d <= top)
            a[2] = v + d;
    }
    rnd_shuffle(a, 3);
    put_ints(a, 3);
}
