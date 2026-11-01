/* Small cases: a few tags, usually from 1 to 5, so that equal tags and
 * chains of partners come up often. Full size: 500 tags, in a shape the
 * seed picks: random; strictly rising (no item has a partner, and the
 * stack holds every item); rising with a low tag at the very end (every
 * partner is the last item); falling with ties; or only a few tag values. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 500;
    int *prices = malloc((size_t)n * sizeof *prices);
    int count[1001] = {0};
    int shape = small ? 0 : (int)rnd(0, 4);
    int top, need, v, i, k;

    if (small) {
        top = rnd_chance(80) ? 5 : 1000;
        for (i = 0; i < n; i++)
            prices[i] = (int)rnd(1, top);
    } else if (shape == 0) {
        for (i = 0; i < n; i++)
            prices[i] = (int)rnd(1, 1000);
    } else if (shape == 1 || shape == 2) {
        /* n distinct tags out of 1..1000, in increasing order. */
        need = n;
        i = 0;
        for (v = 1; v <= 1000 && need > 0; v++)
            if (rnd(1, 1000 - v + 1) <= need) {
                prices[i++] = v;
                need--;
            }
        if (shape == 2)
            prices[n - 1] = (int)rnd(1, prices[0]);
    } else if (shape == 3) {
        for (i = 0; i < n; i++) {
            v = (int)rnd(1, 1000);
            count[v]++;
        }
        i = 0;
        for (v = 1000; v >= 1; v--)
            for (k = 0; k < count[v]; k++)
                prices[i++] = v;
    } else {
        top = (int)rnd(1, 3);
        for (i = 0; i < n; i++)
            prices[i] = 997 + (int)rnd(1, top);
    }
    put_ints(prices, n);
    free(prices);
}
