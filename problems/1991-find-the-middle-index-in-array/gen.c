/* Random values, and often a middle index made at a chosen position p:
 * the values behind p are changed from the far end inwards, then if need
 * be the ones in front of p, each by as much as the bounds allow, until
 * before(p) == after(p). (Each side alone can reach any sum from -top
 * times its length to top times its length, so this always succeeds.) An
 * index left of p may qualify too; the answer is the smallest.
 *
 * Small cases have 1 to 8 values, from [-4, 4], from [-2, 2] (many ties
 * and zeros), from [0, 5] (no negatives), or from the bounds and a few
 * values near 0, with a middle index made 60% of the time. Full-size
 * cases have 100 values, and the seed picks the shape: random values with
 * nothing made (most often no answer, the slowest case for a method that
 * sums both sides at every index); a middle index made at the last
 * position, or at a random one; a run of large positive values, then a
 * run of large negative ones, then a middle index made further right (so
 * before(i) climbs past half the total and falls back); or 99 copies of
 * 1000 or of -1000, whose middle index is 49 with sums of 49000 on each
 * side, past a 16-bit integer. */
static void even_out(int *a, int n, int p, int top)
{
    long long diff = 0, want;
    int i;

    /* diff = before(p) - after(p). */
    for (i = 0; i < p; i++)
        diff += a[i];
    for (i = p + 1; i < n; i++)
        diff -= a[i];
    for (i = n - 1; i > p && diff != 0; i--) {
        want = a[i] + diff;
        if (want > top)
            want = top;
        if (want < -top)
            want = -top;
        diff -= want - a[i];
        a[i] = (int)want;
    }
    for (i = 0; i < p && diff != 0; i++) {
        want = a[i] - diff;
        if (want > top)
            want = top;
        if (want < -top)
            want = -top;
        diff += want - a[i];
        a[i] = (int)want;
    }
}

void gen(int small)
{
    static const int edge[] = {-1000, -999, -1, 0, 1, 999, 1000};
    int nums[100];
    int n, i, kind, v, p, sign;

    if (small) {
        kind = (int)rnd(0, 3);
        n = (int)rnd(1, 8);
        for (i = 0; i < n; i++) {
            if (kind == 0)
                v = (int)rnd(-4, 4);
            else if (kind == 1)
                v = (int)rnd(-2, 2);
            else if (kind == 2)
                v = (int)rnd(0, 5);
            else
                v = edge[rnd(0, 6)];
            nums[i] = v;
        }
        if (rnd_chance(60)) {
            p = (int)rnd(0, n - 1);
            even_out(nums, n, p, 1000);
        }
        put_ints(nums, n);
        return;
    }

    kind = (int)rnd(0, 4);
    n = kind == 4 ? 99 : 100;
    if (kind == 4) {
        sign = rnd_chance(50) ? 1 : -1;
        for (i = 0; i < n; i++)
            nums[i] = 1000 * sign;
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-1000, 1000);
        if (kind == 3) {
            for (i = 0; i < 30; i++)
                nums[i] = (int)rnd(500, 1000);
            for (i = 30; i < 60; i++)
                nums[i] = (int)rnd(-1000, -500);
        }
        if (kind == 1) {
            even_out(nums, n, n - 1, 1000);
        } else if (kind >= 2) {
            p = (int)rnd(kind == 3 ? 60 : 0, n - 1);
            even_out(nums, n, p, 1000);
        }
    }
    put_ints(nums, n);
}
