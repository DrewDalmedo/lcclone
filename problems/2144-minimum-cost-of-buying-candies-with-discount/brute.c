/* Slow and obviously right: try every way of splitting the candies into
 * bundles (the first candy not yet bundled goes alone, with one other, or
 * with two others), and keep the cheapest. In a bundle of three the
 * cheapest candy is the free one, which the rule always allows. */
static int best_split(const int *cost, int n, int *used)
{
    int i, j, k, best = -1, sub, lo;

    for (i = 0; i < n && used[i]; i++)
        ;
    if (i == n)
        return 0;
    used[i] = 1;
    sub = cost[i] + best_split(cost, n, used);
    best = sub;
    for (j = i + 1; j < n; j++) {
        if (used[j])
            continue;
        used[j] = 1;
        sub = cost[i] + cost[j] + best_split(cost, n, used);
        if (sub < best)
            best = sub;
        for (k = j + 1; k < n; k++) {
            if (used[k])
                continue;
            used[k] = 1;
            lo = cost[i];
            if (cost[j] < lo)
                lo = cost[j];
            if (cost[k] < lo)
                lo = cost[k];
            sub = cost[i] + cost[j] + cost[k] - lo + best_split(cost, n, used);
            if (sub < best)
                best = sub;
            used[k] = 0;
        }
        used[j] = 0;
    }
    used[i] = 0;
    return best;
}

int minimumCost(int *cost, int costSize)
{
    int used[100] = {0};

    return best_split(cost, costSize, used);
}
