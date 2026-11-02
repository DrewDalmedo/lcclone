/* One random list of tasks. The seed picks the shape: starts and lengths
 * drawn freely (two times in five); every row the same, often [top, top],
 * the latest possible completion (one in five); or one row completes
 * first, at a chosen time x, and every other row later, so the earliest
 * start and the shortest length usually sit in other rows (two in five).
 * That row is put last half the time. Small cases have 1 to 6 rows, mostly
 * with values of at most 6 so that ties come up often; full-size ones have
 * 100 rows with values from 1 to 100. */
void gen(int small)
{
    int rows[100][2];
    int n = small ? (int)rnd(1, 6) : 100;
    int top = small && rnd_chance(70) ? 6 : 100;
    int shape = (int)rnd(0, 4), x, lo, hi, s, t, w, i;

    if (shape == 2) {
        s = rnd_chance(50) ? top : (int)rnd(1, top);
        t = rnd_chance(50) ? top : (int)rnd(1, top);
        for (i = 0; i < n; i++) {
            rows[i][0] = s;
            rows[i][1] = t;
        }
    } else if (shape >= 3) {
        x = (int)rnd(2, 2 * top - 1);
        w = rnd_chance(50) ? n - 1 : (int)rnd(0, n - 1);
        for (i = 0; i < n; i++) {
            if (i == w) {
                lo = x - top > 1 ? x - top : 1;
                hi = x - 1 < top ? x - 1 : top;
                s = (int)rnd(lo, hi);
                t = x - s;
            } else {
                lo = x + 1 - top > 1 ? x + 1 - top : 1;
                s = (int)rnd(lo, top);
                lo = x + 1 - s > 1 ? x + 1 - s : 1;
                t = (int)rnd(lo, top);
            }
            rows[i][0] = s;
            rows[i][1] = t;
        }
    } else {
        for (i = 0; i < n; i++) {
            rows[i][0] = (int)rnd(1, top);
            rows[i][1] = (int)rnd(1, top);
        }
    }
    put_int_grid(&rows[0][0], n, 2);
}
