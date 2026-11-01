/* A town register, its rows shuffled at the end. Small cases have up to 8
 * people whose lives fall in a window of 2 to 12 years, often at one end
 * of 1950..2050, and now and then a copy of an earlier row, so ties,
 * equal rows and a death in the year of another birth come up often.
 * Full-size cases have 100 people, in a shape the seed picks:
 *   0: lives anywhere in 1950..2050;
 *   1: 2, 4 or 5 groups of short lives, each group the same pattern in a
 *      different decade, so the groups' peaks tie;
 *   2: births late in the range and many deaths in 2050, so the peak is
 *      near 2049;
 *   3: chains of short lives, each person born in the year the one
 *      before died. */
void gen(int small)
{
    static const int group_counts[] = {2, 4, 5};
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? -1 : (int)rnd(0, 3);
    int b[100], d[100], order[100], slot[10], pb[50], pl[50], grid[200];
    int i, j, g, t, w, lo, hi, cur, groups, m;

    if (small) {
        w = (int)rnd(2, 12);
        t = (int)rnd(0, 3);
        lo = t == 0 ? 1950 : t == 1 ? 2050 - w : (int)rnd(1950, 2050 - w);
        for (i = 0; i < n; i++) {
            if (i > 0 && rnd_chance(15)) {
                j = (int)rnd(0, i - 1);
                b[i] = b[j];
                d[i] = d[j];
                continue;
            }
            b[i] = (int)rnd(lo, lo + w - 1);
            d[i] = (int)rnd(b[i] + 1, lo + w);
        }
    } else if (shape == 0) {
        for (i = 0; i < n; i++) {
            b[i] = (int)rnd(1950, 2049);
            d[i] = (int)rnd(b[i] + 1, 2050);
        }
    } else if (shape == 1) {
        groups = group_counts[rnd(0, 2)];
        m = n / groups;
        for (j = 0; j < m; j++) {
            pb[j] = (int)rnd(0, 3);
            pl[j] = (int)rnd(1, 4);
        }
        for (i = 0; i < 10; i++)
            slot[i] = i;
        rnd_shuffle(slot, 10);
        for (g = 0; g < groups; g++)
            for (j = 0; j < m; j++) {
                b[g * m + j] = 1950 + 10 * slot[g] + pb[j];
                d[g * m + j] = b[g * m + j] + pl[j];
            }
    } else if (shape == 2) {
        for (i = 0; i < n; i++) {
            b[i] = (int)rnd(2000, 2049);
            d[i] = rnd_chance(50) ? 2050 : (int)rnd(b[i] + 1, 2050);
        }
    } else {
        cur = (int)rnd(1950, 1970);
        for (i = 0; i < n; i++) {
            if (cur >= 2050 || rnd_chance(10))
                cur = (int)rnd(1950, 2040);
            b[i] = cur;
            hi = b[i] + 5 > 2050 ? 2050 : b[i] + 5;
            d[i] = (int)rnd(b[i] + 1, hi);
            cur = d[i];
        }
    }
    for (i = 0; i < n; i++)
        order[i] = i;
    rnd_shuffle(order, n);
    for (i = 0; i < n; i++) {
        grid[2 * i] = b[order[i]];
        grid[2 * i + 1] = d[order[i]];
    }
    put_int_grid(grid, n, 2);
}
