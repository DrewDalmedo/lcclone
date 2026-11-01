/* Three points, listed in a random order. Small cases keep the coordinates
 * from 0 to 4, so lines and repeated points come up by chance too;
 * full-size ones use 0 to 100. The seed picks the shape: three random
 * points; three points on one line (the third another whole-number point
 * of the line through the first two, before, between or after them); a
 * point repeated; or, at full size, three points that only just miss a
 * line (the third one step off a point of the line). */
static int gcd(int a, int b)
{
    while (b) {
        int t = a % b;

        a = b;
        b = t;
    }
    return a;
}

/* Narrows [*lo, *hi] to the t with 0 <= a + t * s <= top (0 <= a <= top). */
static void clamp(int a, int s, int top, int *lo, int *hi)
{
    int l, h;

    if (s > 0) {
        l = -(a / s);
        h = (top - a) / s;
    } else if (s < 0) {
        l = -((top - a) / -s);
        h = a / -s;
    } else {
        return;
    }
    if (l > *lo)
        *lo = l;
    if (h < *hi)
        *hi = h;
}

void gen(int small)
{
    int top = small ? 4 : 100;
    int p[3][2], order[3] = {0, 1, 2}, out[6];
    int shape = (int)(gen_seed() % (small ? 3 : 4)), i, k;

    for (i = 0; i < 3; i++)
        for (k = 0; k < 2; k++)
            p[i][k] = (int)rnd(0, top);
    if (shape == 1 || shape == 3) {
        int dx = p[1][0] - p[0][0], dy = p[1][1] - p[0][1];
        int g = gcd(dx < 0 ? -dx : dx, dy < 0 ? -dy : dy);
        int lo = -1000, hi = 1000, t;

        if (g > 0) {
            dx /= g;
            dy /= g;
            clamp(p[0][0], dx, top, &lo, &hi);
            clamp(p[0][1], dy, top, &lo, &hi);
            /* t = 0 and t = g give the first two points again, and both
             * are always in range: skip them when there is another. */
            if (hi - lo + 1 > 2) {
                t = (int)rnd(lo, hi - 2);
                if (t >= 0)
                    t++;
                if (t >= g)
                    t++;
            } else {
                t = (int)rnd(lo, hi);
            }
            p[2][0] = p[0][0] + t * dx;
            p[2][1] = p[0][1] + t * dy;
            if (shape == 3) {
                /* Step across the line, not along it. */
                if (dx == 0)
                    k = 0;
                else if (dy == 0)
                    k = 1;
                else
                    k = (int)rnd(0, 1);
                if (p[2][k] == top)
                    p[2][k]--;
                else if (p[2][k] == 0)
                    p[2][k]++;
                else
                    p[2][k] += rnd_chance(50) ? 1 : -1;
            }
        }
    } else if (shape == 2) {
        i = (int)rnd(0, 2);
        k = (int)rnd(0, 2);
        p[k][0] = p[i][0];
        p[k][1] = p[i][1];
    }
    rnd_shuffle(order, 3);
    for (i = 0; i < 3; i++) {
        out[2 * i] = p[order[i]][0];
        out[2 * i + 1] = p[order[i]][1];
    }
    put_int_grid(out, 3, 2);
}
