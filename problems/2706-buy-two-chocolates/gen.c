/* Small cases have 2 to 8 prices, full-size ones 50. The seed picks the
 * range of the prices: 1 to 3 or 1 to 10 (small only, so prices repeat
 * and the cheapest is often there twice), 1 to 100, or 45 to 100 (dear
 * bars, so the cheapest pair costs about money or more); and sometimes
 * sorts them from dearest to cheapest, so the two cheapest come last.
 * money is most often the cost of the two cheapest bars plus a step from
 * -2 to 2 (a miss by a little, an exact fit, or a little left over), kept
 * within 1 to 100; otherwise anything from 1 to 100. */
void gen(int small)
{
    static const int los[] = {1, 1, 1, 45}, his[] = {3, 10, 100, 100};
    int n = small ? (int)rnd(2, 8) : 50;
    int range = small ? (int)rnd(0, 3) : 2 + (int)rnd(0, 1);
    int sorted = rnd_chance(25), first = 101, second = 101, money, i, j, t;
    int *prices = malloc((size_t)n * sizeof *prices);

    for (i = 0; i < n; i++)
        prices[i] = (int)rnd(los[range], his[range]);
    if (sorted)
        for (i = 1; i < n; i++)
            for (j = i; j > 0 && prices[j - 1] < prices[j]; j--) {
                t = prices[j];
                prices[j] = prices[j - 1];
                prices[j - 1] = t;
            }
    for (i = 0; i < n; i++)
        if (prices[i] < first) {
            second = first;
            first = prices[i];
        } else if (prices[i] < second) {
            second = prices[i];
        }
    if (rnd_chance(75)) {
        money = first + second + (int)rnd(-2, 2);
        if (money < 1)
            money = 1;
        if (money > 100)
            money = 100;
    } else {
        money = (int)rnd(1, 100);
    }
    put_ints(prices, n);
    put_int(money);
    free(prices);
}
