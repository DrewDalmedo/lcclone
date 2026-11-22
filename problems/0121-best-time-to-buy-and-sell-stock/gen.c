/* Random prices: a few days for the cross-check with brute.c, else 10^5
 * days, as many as the constraints allow, in a shape the seed picks:
 * random prices from 0 to 10^4; prices that never rise, so the answer is
 * 0; prices that fall from 10^4 to 0 on the second-to-last day and rise on
 * the last, so the best sale is the very last pair and comes after the
 * highest price; or a random walk, so the answer is neither 0 nor 10^4. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i, p;
    int top = small ? 20 : 10000;
    int *prices = malloc((size_t)n * sizeof *prices);

    if (small) {
        for (i = 0; i < n; i++)
            prices[i] = (int)rnd(0, top);
    } else {
        switch (gen_seed() % 4) {
        case 0:
            for (i = 0; i < n; i++)
                prices[i] = (int)rnd(0, top);
            break;
        case 1:
            for (i = 0, p = top; i < n; i++) {
                if (rnd_chance(10))
                    p -= (int)rnd(0, p < 3 ? p : 3);
                prices[i] = p;
            }
            break;
        case 2:
            for (i = 0; i < n - 1; i++)
                prices[i] = (int)((long long)top * (n - 2 - i) / (n - 2));
            prices[n - 1] = (int)rnd(1, top - 1);
            break;
        default:
            for (i = 0, p = (int)rnd(0, top); i < n; i++) {
                p += (int)rnd(-3, 3);
                p = p < 0 ? 0 : p > top ? top : p;
                prices[i] = p;
            }
            break;
        }
    }
    put_ints(prices, n);
    free(prices);
}
