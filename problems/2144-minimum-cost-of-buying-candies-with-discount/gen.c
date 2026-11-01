/* Candy prices. Small cases have up to 8 candies priced 1 to 6, so ties
 * are common; full-size ones have 98 to 100 candies (every remainder mod
 * 3), in a shape picked by the seed: random, ascending, descending, all
 * one price, or a few prices including 1 and 100. */
static int cmp_up(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

void gen(int small)
{
    int cost[100], n, shape, top, pick, i;

    if (small) {
        n = (int)rnd(1, 8);
        top = (int)rnd(1, 6);
        for (i = 0; i < n; i++)
            cost[i] = (int)rnd(1, top);
        put_ints(cost, n);
        return;
    }
    n = (int)rnd(98, 100);
    shape = (int)rnd(0, 4);
    for (i = 0; i < n; i++)
        cost[i] = (int)rnd(1, 100);
    if (shape == 1 || shape == 2) {
        qsort(cost, (size_t)n, sizeof *cost, cmp_up);
        if (shape == 2)
            for (i = 0; i < n / 2; i++) {
                pick = cost[i];
                cost[i] = cost[n - 1 - i];
                cost[n - 1 - i] = pick;
            }
    } else if (shape == 3) {
        pick = (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            cost[i] = pick;
    } else if (shape == 4) {
        for (i = 0; i < n; i++) {
            pick = (int)rnd(0, 3);
            cost[i] = pick == 0 ? 1 : pick == 1 ? 100 : pick == 2 ? 50 : 51;
        }
    }
    put_ints(cost, n);
}
