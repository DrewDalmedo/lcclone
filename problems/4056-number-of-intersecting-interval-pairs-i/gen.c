/* Intervals lie within 0 to top: top is 1 to 12 in small cases, with 2 to
 * 7 intervals, and 100 in full-size ones, with 100 intervals (or, now and
 * then, any number from 2 to 99). The seed picks the shape:
 *   0: random intervals, mostly short (single points included); now and
 *      then one is a copy of an earlier one, or starts where an earlier
 *      one ends or just after it, or ends where an earlier one starts or
 *      just before it;
 *   1: intervals around one point, nested or crossing, and random ones;
 *   2: full size only: pairwise disjoint intervals (the answer is 0), or
 *      intervals that all hold one point (the answer is n(n - 1) / 2).
 * The intervals are then listed in a random order. */
static int top, longest, s[100], e[100];

static int smaller(int a, int b)
{
    return a < b ? a : b;
}

static void random_interval(int k)
{
    int len = (int)rnd(0, smaller(longest, top));

    s[k] = (int)rnd(0, top - len);
    e[k] = s[k] + len;
}

/* Interval k beside interval j: touching it or one point away from it. */
static void next_to(int k, int j)
{
    int how = (int)rnd(0, 4), at, len;

    if (how == 1 || how == 2) {
        at = how == 1 ? e[j] : e[j] + 1;
        if (at <= top) {
            len = (int)rnd(0, smaller(longest, top - at));
            s[k] = at;
            e[k] = at + len;
            return;
        }
    } else if (how == 3 || how == 4) {
        at = how == 3 ? s[j] : s[j] - 1;
        if (at >= 0) {
            len = (int)rnd(0, smaller(longest, at));
            e[k] = at;
            s[k] = at - len;
            return;
        }
    }
    s[k] = s[j];
    e[k] = e[j];
}

void gen(int small)
{
    int n, shape, v, k, j, p, i, x;
    int order[100], flat[200], mark[101];

    n = small ? (int)rnd(2, 7) : (rnd_chance(80) ? 100 : (int)rnd(2, 99));
    top = small ? (int)rnd(1, 12) : 100;
    longest = small ? 4 : (int)rnd(3, 40);
    v = (int)rnd(0, small ? 9 : 11);
    shape = v < 7 ? 0 : v < 10 ? 1 : 2;
    if (shape == 0) {
        for (k = 0; k < n; k++) {
            if (k > 0 && rnd_chance(35)) {
                j = (int)rnd(0, k - 1);
                next_to(k, j);
            } else {
                random_interval(k);
            }
        }
    } else if (shape == 1) {
        p = (int)rnd(0, top);
        for (k = 0; k < n; k++) {
            if (rnd_chance(60)) {
                s[k] = (int)rnd(p - smaller(longest, p), p);
                e[k] = (int)rnd(p, p + smaller(longest, top - p));
            } else {
                random_interval(k);
            }
        }
    } else if (rnd_chance(50)) {
        /* n different starts, each interval ending before the next start. */
        for (x = 0; x <= 100; x++)
            mark[x] = 0;
        for (k = 0; k < n; k++) {
            do
                x = (int)rnd(0, 100);
            while (mark[x]);
            mark[x] = 1;
        }
        k = 0;
        for (x = 0; x <= 100; x++)
            if (mark[x])
                s[k++] = x;
        for (k = 0; k < n; k++) {
            j = k + 1 < n ? s[k + 1] - 1 : 100;
            e[k] = (int)rnd(s[k], j);
        }
    } else {
        p = (int)rnd(0, 100);
        for (k = 0; k < n; k++) {
            s[k] = (int)rnd(p - smaller(longest, p), p);
            e[k] = (int)rnd(p, p + smaller(longest, 100 - p));
        }
    }
    for (i = 0; i < n; i++)
        order[i] = i;
    rnd_shuffle(order, n);
    for (i = 0; i < n; i++) {
        flat[2 * i] = s[order[i]];
        flat[2 * i + 1] = e[order[i]];
    }
    put_int_grid(flat, n, 2);
}
