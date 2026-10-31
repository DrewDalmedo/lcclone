/* Slow and obviously right: every pair of a buying and a selling day. */
int maxProfit(int *prices, int pricesSize)
{
    int best = 0, i, k;

    for (i = 0; i < pricesSize; i++)
        for (k = i + 1; k < pricesSize; k++)
            if (prices[k] - prices[i] > best)
                best = prices[k] - prices[i];
    return best;
}
