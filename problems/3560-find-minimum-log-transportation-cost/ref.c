/* Reference solution: three trucks allow at most one cut, and only a log
 * longer than k needs one. Cutting a log of length L into a and L - a costs
 * a * (L - a), which is smallest when the pieces are as unequal as allowed:
 * k and L - k. The product can pass 2^31, so it is formed in 64 bits. */
long long minCuttingCost(int n, int m, int k)
{
    long long cost = 0;

    if (n > k)
        cost += (long long)k * (n - k);
    if (m > k)
        cost += (long long)k * (m - k);
    return cost;
}
