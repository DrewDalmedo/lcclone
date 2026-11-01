/* Reference solution: dynamic programming from the start. The least you
 * can have paid on landing on pad i (pad n being the finish) is the
 * cheaper of launching from pad i - 1 and launching from pad i - 2; pads
 * 0 and 1 cost nothing to stand on. Only the last two of these values are
 * kept. */
int minCostClimbingStairs(int *cost, int costSize)
{
    int two_back = 0, one_back = 0, here, a, b, i;

    for (i = 2; i <= costSize; i++) {
        a = one_back + cost[i - 1];
        b = two_back + cost[i - 2];
        here = a < b ? a : b;
        two_back = one_back;
        one_back = here;
    }
    return one_back;
}
