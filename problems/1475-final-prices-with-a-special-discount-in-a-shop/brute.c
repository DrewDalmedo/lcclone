/* Slow and obviously right: for each item, walk right until the first tag
 * that is at most its own, and take that tag off. */
int *finalPrices(int *prices, int pricesSize, int *returnSize)
{
    int *out = malloc((size_t)pricesSize * sizeof *out);
    int i, j;

    for (i = 0; i < pricesSize; i++) {
        out[i] = prices[i];
        for (j = i + 1; j < pricesSize; j++)
            if (prices[j] <= prices[i]) {
                out[i] = prices[i] - prices[j];
                break;
            }
    }
    *returnSize = pricesSize;
    return out;
}
