/* Reference solution: count the prices (they are at most 100), then walk
 * them from the dearest down. Bundling greedily in threes from the top,
 * every third candy in that order (the 3rd, 6th, ...) is free. */
int minimumCost(int *cost, int costSize)
{
    int count[101] = {0}, total = 0, rank = 0, price, i;

    for (i = 0; i < costSize; i++)
        count[cost[i]]++;
    for (price = 100; price >= 1; price--)
        for (i = 0; i < count[price]; i++) {
            rank++;
            if (rank % 3 != 0)
                total += price;
        }
    return total;
}
