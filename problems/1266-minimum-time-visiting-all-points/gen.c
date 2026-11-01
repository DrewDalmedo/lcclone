/* Small cases have 1 to 6 points with coordinates from -3 to 3, so
 * repeats, straight and diagonal legs come up often; full-size ones have
 * 100 points from -1000 to 1000. The seed picks the shape (small cases
 * pick random points more often, 10 times in 14):
 *   0-2: random points;
 *   3:   points alternating between two opposite corners of the square
 *        (at full size the largest answer, 198000);
 *   4:   points on one straight, diagonal or crossing line, in a random
 *        order, so later points lie on the paths of earlier legs;
 *   5:   short random steps (each coordinate changes by -3 to 3, or by
 *        -1 to 1 for small cases), with repeats;
 *   6:   one point, over and over. */
static void narrow(int at, int dir, int top, int *lo, int *hi)
{
    if (dir == 1) {
        if (-top - at > *lo) *lo = -top - at;
        if (top - at < *hi) *hi = top - at;
    } else if (dir == -1) {
        if (at - top > *lo) *lo = at - top;
        if (at + top < *hi) *hi = at + top;
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 100, top = small ? 3 : 1000;
    int shape = (int)rnd(0, small ? 13 : 6), step = small ? 1 : 3;
    int i, x, y, t, sx, sy, d, lo, hi;
    int *p = malloc((size_t)2 * n * sizeof *p);

    if (shape > 6)
        shape = 0;
    x = (int)rnd(-top, top);
    y = (int)rnd(-top, top);
    sx = (int)rnd(-1, 1);
    sy = sx == 0 ? 1 : (int)rnd(-1, 1);
    d = rnd_chance(50) ? 1 : -1;
    lo = -2 * top;
    hi = 2 * top;
    narrow(x, sx, top, &lo, &hi);
    narrow(y, sy, top, &lo, &hi);
    for (i = 0; i < n; i++) {
        if (shape == 3) {
            p[2 * i] = i % 2 ? d * top : -d * top;
            p[2 * i + 1] = i % 2 ? top : -top;
        } else if (shape == 4) {
            t = (int)rnd(lo, hi);
            p[2 * i] = x + t * sx;
            p[2 * i + 1] = y + t * sy;
        } else if (shape == 5) {
            if (i > 0) {
                t = (int)rnd(-step, step);
                x += t;
                t = (int)rnd(-step, step);
                y += t;
                if (x < -top) x = -top;
                if (x > top) x = top;
                if (y < -top) y = -top;
                if (y > top) y = top;
            }
            p[2 * i] = x;
            p[2 * i + 1] = y;
        } else if (shape == 6) {
            p[2 * i] = x;
            p[2 * i + 1] = y;
        } else {
            p[2 * i] = (int)rnd(-top, top);
            p[2 * i + 1] = (int)rnd(-top, top);
        }
    }
    put_int_grid(p, n, 2);
    free(p);
}
