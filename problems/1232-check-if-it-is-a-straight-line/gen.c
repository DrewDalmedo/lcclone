/* Distinct points. Small cases have 2 to 6 points with coordinates from
 * -6 to 6 (but see shape 3); full-size ones have 1000 points anywhere
 * from -10^4 to 10^4. The seed picks a shape:
 *  0. points on one line, slanted, vertical or horizontal, in a random
 *     order, the two farthest apart among them: true;
 *  1. the same with one point moved off the line by 1, most often the
 *     last or the third point listed: false (true for two points);
 *  2. small only: points anywhere, mostly false;
 *  3. small only: three points far apart, the third the whole-number
 *     point nearest the line through the other two, so most often off
 *     it by a hair. */

static int inbox(long long x, long long y, int box)
{
    return x >= -box && x <= box && y >= -box && y <= box;
}

/* floor(a / b) for b > 0 */
static long long floordiv(long long a, long long b)
{
    long long q = a / b;

    return q * b > a ? q - 1 : q;
}

/* Up to n points on one line into xs, ys; returns how many (at least 2). */
static int line(int *xs, int *ys, int n, int box, int step, int small)
{
    int dx, dy, px, py, kind, count, i, j, t;
    long long klo, khi;
    int *ks;

    for (;;) {
        kind = (int)rnd(0, 3);
        dx = kind == 2 ? 0 : (int)rnd(-step, step);
        dy = kind == 3 ? 0 : (int)rnd(-step, step);
        if (dx == 0 && dy == 0)
            continue;
        px = (int)rnd(-box, box);
        py = (int)rnd(-box, box);
        klo = 0;
        while (inbox(px + (klo - 1) * dx, py + (klo - 1) * dy, box))
            klo--;
        khi = 0;
        while (inbox(px + (khi + 1) * dx, py + (khi + 1) * dy, box))
            khi++;
        count = (int)(khi - klo + 1);
        if (count >= (small ? 2 : n))
            break;
    }
    if (n > count)
        n = count;
    /* the two ends of the line, then n - 2 other steps at random */
    ks = malloc((size_t)count * sizeof *ks);
    for (i = 0; i < count; i++)
        ks[i] = (int)(klo + i);
    t = ks[1];
    ks[1] = ks[count - 1];
    ks[count - 1] = t;
    rnd_shuffle(ks + 2, count - 2);
    rnd_shuffle(ks, n);
    for (i = 0; i < n; i++) {
        j = ks[i];
        xs[i] = px + j * dx;
        ys[i] = py + j * dy;
    }
    free(ks);
    return n;
}

/* Three points: two anywhere, at least 2 apart in x or y, and the
 * whole-number point nearest their line strictly between them. */
static void nearmiss(int *xs, int *ys, int box)
{
    int x0, y0, x1, y1, ax, ay, t, swap, i;
    long long num, den, u, v;

    do {
        x0 = (int)rnd(-box, box);
        y0 = (int)rnd(-box, box);
        x1 = (int)rnd(-box, box);
        y1 = (int)rnd(-box, box);
        ax = x1 > x0 ? x1 - x0 : x0 - x1;
        ay = y1 > y0 ? y1 - y0 : y0 - y1;
    } while (ax < 2 && ay < 2);
    swap = ay > ax; /* steep: work with x and y swapped */
    if (swap) {
        t = x0, x0 = y0, y0 = t;
        t = x1, x1 = y1, y1 = t;
    }
    u = rnd(x0 < x1 ? x0 + 1 : x1 + 1, x0 < x1 ? x1 - 1 : x0 - 1);
    num = (u - x0) * (y1 - y0);
    den = x1 - x0;
    if (den < 0) {
        num = -num;
        den = -den;
    }
    v = y0 + floordiv(2 * num + den, 2 * den);
    xs[0] = x0, ys[0] = y0;
    xs[1] = x1, ys[1] = y1;
    xs[2] = (int)u, ys[2] = (int)v;
    if (swap)
        for (i = 0; i < 3; i++) {
            t = xs[i], xs[i] = ys[i], ys[i] = t;
        }
}

void gen(int small)
{
    int box = small ? 6 : 10000, want = small ? (int)rnd(2, 6) : 1000;
    int shape = (int)rnd(0, small ? 3 : 1), n = want, i, j, w, x, y;
    int *xs = malloc(1000 * sizeof *xs), *ys = malloc(1000 * sizeof *ys);
    int *order = malloc(1000 * sizeof *order), *grid;
    char *used;

    if (shape <= 1) {
        n = line(xs, ys, want, box, small ? 3 : 10, small);
        if (shape == 1) {
            w = (int)rnd(0, 3);
            j = w <= 1 ? n - 1 : w == 2 && n > 2 ? 2 : (int)rnd(0, n - 1);
            if (xs[0] == xs[1]) /* a vertical line: move sideways */
                xs[j] += xs[j] < box ? 1 : -1;
            else
                ys[j] += ys[j] < box ? 1 : -1;
        }
    } else if (shape == 2) {
        used = calloc((size_t)(2 * box + 1) * (2 * box + 1), 1);
        for (i = 0; i < n; i++) {
            do {
                x = (int)rnd(-box, box);
                y = (int)rnd(-box, box);
            } while (used[(x + box) * (2 * box + 1) + y + box]);
            used[(x + box) * (2 * box + 1) + y + box] = 1;
            xs[i] = x;
            ys[i] = y;
        }
        free(used);
    } else {
        n = 3;
        nearmiss(xs, ys, 10000);
    }
    for (i = 0; i < n; i++)
        order[i] = i;
    if (shape == 3)
        rnd_shuffle(order, n);
    grid = malloc((size_t)n * 2 * sizeof *grid);
    for (i = 0; i < n; i++) {
        grid[2 * i] = xs[order[i]];
        grid[2 * i + 1] = ys[order[i]];
    }
    put_int_grid(grid, n, 2);
    free(grid);
    free(xs);
    free(ys);
    free(order);
}
