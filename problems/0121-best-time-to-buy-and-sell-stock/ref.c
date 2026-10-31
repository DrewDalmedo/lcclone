/* Reference solution: one pass, remembering the lowest price so far. */
int maxProfit(int *prices, int pricesSize)
{
    int low = prices[0], best = 0, i;

    for (i = 1; i < pricesSize; i++) {
        if (prices[i] - low > best)
            best = prices[i] - low;
        if (prices[i] < low)
            low = prices[i];
    }
    return best;
}
