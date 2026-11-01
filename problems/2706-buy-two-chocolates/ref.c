/* Reference solution: any affordable pair costs at least as much as the
 * two cheapest bars together, so find those two in one pass (a repeated
 * price counts twice) and buy them if they fit within money. */
int buyChoco(int *prices, int pricesSize, int money)
{
    int first = INT_MAX, second = INT_MAX, i;

    for (i = 0; i < pricesSize; i++) {
        if (prices[i] < first) {
            second = first;
            first = prices[i];
        } else if (prices[i] < second) {
            second = prices[i];
        }
    }
    return first + second <= money ? money - (first + second) : money;
}
