/* A row with no two flowers side by side. Mostly n is the most new flowers
 * that fit or one more, so the answer turns on a single flower and a
 * greedy pass must go on to the last free place in the row; otherwise n
 * is 0 or anything up to the row's length. Small cases: up to 10 plots,
 * bare, sparse or dense. Full-size cases, in a shape the seed picks: a
 * bare row of 2 * 10^4 or 2 * 10^4 - 1 plots (room for 10^4); random
 * flowers; flowers in every other plot; or bare stretches of up to 600
 * plots between single flowers. */

/* The most new flowers that fit, planting greedily from the left. */
static int room(const int *bed, int len)
{
    int count = 0, before = 0, i;

    for (i = 0; i < len; i++) {
        if (bed[i])
            before = 1;
        else if (!before && (i + 1 == len || !bed[i + 1])) {
            count++;
            before = 1;
        } else
            before = 0;
    }
    return count;
}

void gen(int small)
{
    int len = small ? (int)rnd(1, 10) : 20000;
    int shape = (int)rnd(0, 4), *bed, i, k = 0, percent = 0, cap, n, pick;

    if (!small && shape == 0 && rnd_chance(50))
        len--;
    bed = malloc((size_t)len * sizeof *bed);
    if (small) {
        k = (int)rnd(0, 3);
        percent = k * 25;
    } else if (shape == 1) {
        percent = (int)rnd(5, 60);
    }
    for (i = 0; i < len; i++)
        bed[i] = 0;
    if (small || shape == 1) {
        for (i = 0; i < len; i++)
            if (i == 0 || !bed[i - 1])
                bed[i] = rnd_chance(percent);
    } else if (shape == 2) {
        k = (int)rnd(0, 1);
        for (i = k; i < len; i += 2)
            bed[i] = 1;
    } else if (shape >= 3) {
        /* A flower after each bare stretch, the stretch lengths random. */
        for (i = (int)rnd(0, 600); i < len; i += 1 + k) {
            bed[i] = 1;
            k = (int)rnd(1, 600);
        }
    }
    cap = room(bed, len);
    pick = (int)rnd(0, 9);
    if (pick == 0)
        n = 0;
    else if (pick == 1)
        n = (int)rnd(0, len);
    else if (pick <= 5)
        n = cap;
    else
        n = cap + 1;
    if (n > len)
        n = len;
    put_ints(bed, len);
    put_int(n);
    free(bed);
}
