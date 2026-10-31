/* Random prices: a few days for the cross-check with brute.c, else as
 * many days as the constraints allow. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i;
    int top = small ? 20 : 10000;
    int *prices = malloc((size_t)n * sizeof *prices);

    for (i = 0; i < n; i++)
        prices[i] = (int)rnd(0, top);
    put_ints(prices, n);
    free(prices);
}
