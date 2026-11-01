/* A position and some points. Small cases keep every coordinate within
 * 1..5, so that points in line, ties, copies and the position itself come
 * up often. Full-size ones have 10^4 points and coordinates up to 10^4;
 * the seed picks all of them on the vertical line through (x, y) with the
 * nearest last, none in line, or only one in line at the far end of its
 * line (distance 9999) among nearer points that are not. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000;
    int top = small ? 5 : 10000;
    int shape = small ? -1 : (int)rnd(0, 2);
    int *p = malloc((size_t)n * 2 * sizeof *p);
    int x, y, i, a, b;

    if (shape == 2) {
        x = 1;
        y = (int)rnd(1, top);
    } else {
        x = (int)rnd(1, top);
        y = (int)rnd(1, top);
    }
    for (i = 0; i < n; i++) {
        if (shape < 0) {
            a = (int)rnd(1, top);
            b = (int)rnd(1, top);
        } else if (shape == 0) {
            /* On the line a == x, never on the row b == y, so the last
             * point, placed at (x, y), is the only nearest one. */
            a = x;
            do
                b = (int)rnd(1, top);
            while (b == y);
            if (i == n - 1)
                b = y;
        } else {
            /* Neither coordinate shared: shifted by 1..top-1, wrapping. */
            a = (int)rnd(1, top - 1);
            a = (x - 1 + a) % top + 1;
            b = (int)rnd(1, top - 1);
            b = (y - 1 + b) % top + 1;
            if (shape == 2 && i == n - 1) {
                a = top;
                b = y;
            }
        }
        p[2 * i] = a;
        p[2 * i + 1] = b;
    }
    put_int(x);
    put_int(y);
    put_int_grid(p, n, 2);
    free(p);
}
