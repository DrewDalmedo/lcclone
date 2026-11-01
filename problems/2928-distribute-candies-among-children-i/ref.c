/* Reference solution: inclusion-exclusion. Without the cap there are
 * C(m + 2, 2) ways to split m candies among three children. Subtract the
 * ways where some child gets at least limit + 1 (give that child limit + 1
 * first and split the rest freely), add back those where two children do,
 * and subtract those where all three do. */
static int ways(int m)
{
    return m < 0 ? 0 : (m + 1) * (m + 2) / 2;
}

int distributeCandies(int n, int limit)
{
    int k = limit + 1;

    return ways(n) - 3 * ways(n - k) + 3 * ways(n - 2 * k) - ways(n - 3 * k);
}
