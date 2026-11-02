/* A target and some drones. Small cases keep every coordinate within
 * -3..3 and give many drones a range equal to their distance, one less or
 * one more, so that ties, drones on the target and both sides of the
 * range limit come up often. Full-size ones have 100 drones anywhere in
 * -25..25; the seed picks random ranges, every drone just out of range
 * but the last one, or the target in a corner with drones crowding the
 * opposite corner (distances near 100, ranges near their distance). */
static int dist(int x, int y, int tx, int ty)
{
    return abs(x - tx) + abs(y - ty);
}

void gen(int small)
{
    int m = small ? (int)rnd(1, 8) : 100;
    int top = small ? 3 : 25;
    int shape = small ? 0 : (int)rnd(0, 2);
    int *d = malloc((size_t)m * 3 * sizeof *d);
    int tx, ty, i, x, y, r, k, t[2];

    if (shape == 2) {
        tx = top;
        ty = top;
    } else {
        tx = (int)rnd(-top, top);
        ty = (int)rnd(-top, top);
    }
    for (i = 0; i < m; i++) {
        do {
            if (shape == 2) {
                x = (int)rnd(-top, -top + 3);
                y = (int)rnd(-top, -top + 3);
            } else {
                x = (int)rnd(-top, top);
                y = (int)rnd(-top, top);
            }
            k = dist(x, y, tx, ty);
            /* A range of k - 1 needs k >= 2 to stay at least 1. */
        } while (shape == 1 && i < m - 1 && k < 2);
        if (shape == 1) {
            /* One short of the trip; the last drone just makes it. */
            r = i == m - 1 ? k : k - 1;
        } else if (rnd_chance(small ? 60 : 80)) {
            r = k + (int)rnd(-1, 1);
        } else {
            r = (int)rnd(1, small ? 8 : 100);
        }
        if (r < 1)
            r = 1;
        if (r > 100)
            r = 100;
        d[3 * i] = x;
        d[3 * i + 1] = y;
        d[3 * i + 2] = r;
    }
    t[0] = tx;
    t[1] = ty;
    put_int_grid(d, m, 3);
    put_ints(t, 2);
    free(d);
}
