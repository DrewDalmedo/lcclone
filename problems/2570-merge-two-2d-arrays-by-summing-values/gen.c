/* Two tallies with ids picked in increasing order from a range. Small
 * cases use 1 to 5 rows with ids up to 8, so shared ids, tails and lists
 * that lie wholly before the other come up often; now and then the ids sit
 * at the top of the range, up to 1000, and the values are 1000. Full size:
 * 200 rows in each tally (or 200 and a random count), in a shape the seed
 * picks: ids anywhere in 1..1000; the same ids in both; no id shared, the
 * two interleaved; or all of nums1 below all of nums2. */
static void pick_ids(int *ids, int n, int lo, int hi, int step)
{
    int need = n, left = (hi - lo) / step + 1, id, r;

    for (id = lo; need > 0; id += step, left--) {
        r = (int)rnd(1, left);
        if (r <= need)
            ids[n - need--] = id;
    }
}

static void put_tally(const int *ids, int n, int small, int big)
{
    int rows[200][2], i;

    for (i = 0; i < n; i++) {
        rows[i][0] = ids[i];
        if (big)
            rows[i][1] = 1000;
        else
            rows[i][1] = (int)rnd(1, small ? 9 : 1000);
    }
    put_int_grid(&rows[0][0], n, 2);
}

void gen(int small)
{
    int ids1[200], ids2[200], n1, n2, shape, base = 0, big, i;

    if (small) {
        n1 = (int)rnd(1, 5);
        n2 = (int)rnd(1, 5);
        if (rnd_chance(15))
            base = 992;
        big = rnd_chance(15);
        pick_ids(ids1, n1, base + 1, base + 8, 1);
        pick_ids(ids2, n2, base + 1, base + 8, 1);
        put_tally(ids1, n1, 1, big);
        put_tally(ids2, n2, 1, big);
        return;
    }
    shape = (int)rnd(0, 3);
    n1 = 200;
    n2 = rnd_chance(50) ? 200 : (int)rnd(1, 200);
    big = rnd_chance(20);
    if (shape == 0) {
        pick_ids(ids1, n1, 1, 1000, 1);
        pick_ids(ids2, n2, 1, 1000, 1);
    } else if (shape == 1) {
        n2 = n1;
        pick_ids(ids1, n1, 1, 1000, 1);
        for (i = 0; i < n1; i++)
            ids2[i] = ids1[i];
    } else if (shape == 2) {
        pick_ids(ids1, n1, 1, 999, 2);
        pick_ids(ids2, n2, 2, 1000, 2);
    } else {
        pick_ids(ids1, n1, 1, 500, 1);
        pick_ids(ids2, n2, 501, 1000, 1);
    }
    put_tally(ids1, n1, 0, big);
    put_tally(ids2, n2, 0, big);
}
