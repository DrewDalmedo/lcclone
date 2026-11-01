/* Slow and obviously right: for each log, try leaving it whole (a = 0) or
 * cutting it after every length a, and keep the cheapest choice that makes
 * at most three pieces, none longer than k. A second cut of a piece would
 * make a fourth piece, so trying one cut per log at most is enough. */
long long minCuttingCost(int n, int m, int k)
{
    long long best = -1, cost;
    int a, b, pieces, fits;

    for (a = 0; a < n; a++)
        for (b = 0; b < m; b++) {
            pieces = 2 + (a > 0) + (b > 0);
            fits = (a > 0 ? a <= k && n - a <= k : n <= k) &&
                   (b > 0 ? b <= k && m - b <= k : m <= k);
            if (pieces > 3 || !fits)
                continue;
            cost = (long long)a * (n - a) + (long long)b * (m - b);
            if (best < 0 || cost < best)
                best = cost;
        }
    return best;
}
