/* Slow and obviously right: try every pair of two different bars, and
 * keep the cheapest total that money covers. */
int buyChoco(int *prices, int pricesSize, int money)
{
    int best = -1, i, j, total;

    for (i = 0; i < pricesSize; i++)
        for (j = i + 1; j < pricesSize; j++) {
            total = prices[i] + prices[j];
            if (total <= money && (best < 0 || total < best))
                best = total;
        }
    return best < 0 ? money : money - best;
}
