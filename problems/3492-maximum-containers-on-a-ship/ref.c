/* Reference solution: the weight allows maxWeight / w containers (whole
 * division, rounding down), the deck n * n; the answer is the smaller.
 * Integer division only: a float quotient can round up near 10^9. */
int maxContainers(int n, int w, int maxWeight)
{
    int byWeight = maxWeight / w, cells = n * n;

    return byWeight < cells ? byWeight : cells;
}
