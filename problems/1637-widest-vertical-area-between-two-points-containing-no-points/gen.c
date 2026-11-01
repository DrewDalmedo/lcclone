/* Small cases have 2 to 8 points: mostly with both coordinates from
 * 0..12, so that x-coordinates repeat and gaps tie; some with every x
 * equal; some over the whole range, x = 0 and x = 10^9 often included;
 * some with x close together far from 0 and y spread over the whole range.
 * Full-size ones have 10^5 points, with the seed picking the shape of the
 * x-coordinates: random over the whole range, 0 and 10^9 included; the
 * same sorted increasing or decreasing; all equal; or a few distinct
 * values. The y-coordinates are always random over the whole range. */
#define TOP 1000000000

static int cmp_up(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

static int cmp_down(const void *a, const void *b)
{
    return cmp_up(b, a);
}

void gen(int small)
{
    int n, shape, i, base, x, y;
    int *xs, *pts;

    if (small) {
        n = rnd_chance(15) ? 2 : (int)rnd(3, 8);
        shape = (int)rnd(0, 9);
    } else {
        n = 100000;
        shape = (int)rnd(0, 4);
    }
    xs = malloc((size_t)n * sizeof *xs);
    pts = malloc((size_t)n * 2 * sizeof *pts);
    base = (int)rnd(0, TOP - 20);
    for (i = 0; i < n; i++) {
        if (small && shape <= 5)
            x = (int)rnd(0, 12);
        else if (small && shape == 6)
            x = base;
        else if (small && shape <= 8)
            x = (int)rnd(0, TOP);
        else if (small)
            x = base + (int)rnd(0, 20);
        else if (shape == 3)
            x = base;
        else if (shape == 4)
            x = (int)rnd(0, 30) * 30000000;
        else
            x = (int)rnd(0, TOP);
        xs[i] = x;
    }
    if (small && (shape == 7 || shape == 8)) {
        if (rnd_chance(50))
            xs[0] = 0;
        if (rnd_chance(50))
            xs[n - 1] = TOP;
        rnd_shuffle(xs, n);
    } else if (!small && shape <= 2) {
        xs[0] = 0;
        xs[1] = TOP;
        if (shape == 1)
            qsort(xs, (size_t)n, sizeof *xs, cmp_up);
        else if (shape == 2)
            qsort(xs, (size_t)n, sizeof *xs, cmp_down);
        else
            rnd_shuffle(xs, n);
    }
    for (i = 0; i < n; i++) {
        if (small && shape <= 5)
            y = (int)rnd(0, 12);
        else
            y = (int)rnd(0, TOP);
        pts[2 * i] = xs[i];
        pts[2 * i + 1] = y;
    }
    put_int_grid(pts, n, 2);
    free(xs);
    free(pts);
}
