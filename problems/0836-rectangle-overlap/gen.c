/* Two plots. Small cases have every coordinate from -3 to 3, so plots
 * often touch along a side or at a corner, cross or contain one another.
 * Full-size ones use the whole range, -10^9 to 10^9: plots placed anywhere
 * (their shared area, when there is one, is mostly far too large for an
 * int), plots that end at the same large coordinate along one axis or
 * miss it by one either way, one plot inside the other, or a plot that is
 * the whole range. */

/* Picks a span lo <= *a < *b <= hi. */
static void span(long long lo, long long hi, int *a, int *b)
{
    *a = (int)rnd(lo, hi - 1);
    *b = (int)rnd(*a + 1, hi);
}

void gen(int small)
{
    const long long lim = small ? 3 : 1000000000;
    int r[2][4], t[4], shape = (int)rnd(0, 9), axis, k, s, d;

    for (k = 0; k < 2; k++)
        for (axis = 0; axis < 2; axis++)
            span(-lim, lim, &r[k][axis], &r[k][axis + 2]);
    if (!small && shape >= 3 && shape < 6) {
        axis = (int)rnd(0, 1);
        s = (int)rnd(-lim + 2, lim - 2);
        d = (int)rnd(-1, 1);
        r[0][axis] = (int)rnd(-lim, s - 2);
        r[0][axis + 2] = s + d;
        r[1][axis] = s;
        r[1][axis + 2] = (int)rnd(s + 1, lim);
    } else if (!small && shape >= 6 && shape < 9) {
        for (axis = 0; axis < 2; axis++)
            span(r[0][axis], r[0][axis + 2], &r[1][axis], &r[1][axis + 2]);
    } else if (!small && shape == 9) {
        r[0][0] = r[0][1] = (int)-lim;
        r[0][2] = r[0][3] = (int)lim;
        if (rnd_chance(50))
            memcpy(r[1], r[0], sizeof r[0]);
    }
    if (rnd_chance(50)) {
        memcpy(t, r[0], sizeof t);
        memcpy(r[0], r[1], sizeof t);
        memcpy(r[1], t, sizeof t);
    }
    put_ints(r[0], 4);
    put_ints(r[1], 4);
}
