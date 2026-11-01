/* Distinct random points. Small cases use a few points in a small square,
 * so collinear triples come up often, and one time in five put every
 * point on one line. Full-size ones have 50 points over the whole range. */

static int taken(const int *pts, int n, int x, int y)
{
    int i;

    for (i = 0; i < n; i++)
        if (pts[2 * i] == x && pts[2 * i + 1] == y)
            return 1;
    return 0;
}

void gen(int small)
{
    int n = small ? (int)rnd(3, 7) : 50, i = 0, x, y;
    int r = small ? (int)rnd(1, 5) : 50;
    int *pts = malloc((size_t)(2 * n) * sizeof *pts);

    if (small && rnd_chance(20)) {
        /* All on the line through (bx, by) with step (dx, dy). */
        int bx = (int)rnd(-5, 5), by = (int)rnd(-5, 5), dx, dy, t;

        do {
            dx = (int)rnd(-2, 2);
            dy = (int)rnd(-2, 2);
        } while (dx == 0 && dy == 0);
        while (i < n) {
            t = (int)rnd(-5, 5);
            x = bx + t * dx;
            y = by + t * dy;
            if (!taken(pts, i, x, y)) {
                pts[2 * i] = x;
                pts[2 * i + 1] = y;
                i++;
            }
        }
    } else {
        while (i < n) {
            x = (int)rnd(-r, r);
            y = (int)rnd(-r, r);
            if (!taken(pts, i, x, y)) {
                pts[2 * i] = x;
                pts[2 * i + 1] = y;
                i++;
            }
        }
    }
    put_int_grid(pts, n, 2);
    free(pts);
}
