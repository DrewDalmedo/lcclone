/* Slow and obviously right: follow every route, from both starting pads
 * and with both kinds of launch from every pad, adding up what each one
 * pays, and keep the cheapest. */
static void walk(const int *cost, int n, int pad, int paid, int *best)
{
    if (pad >= n) {
        if (paid < *best)
            *best = paid;
        return;
    }
    walk(cost, n, pad + 1, paid + cost[pad], best);
    walk(cost, n, pad + 2, paid + cost[pad], best);
}

int minCostClimbingStairs(int *cost, int costSize)
{
    int best = INT_MAX;

    walk(cost, costSize, 0, 0, &best);
    walk(cost, costSize, 1, 0, &best);
    return best;
}
