/* Reference solution: ways(n) = ways(n - 1) + ways(n - 2), since the last
 * move is one step or two, computed from the bottom up. */
int climbStairs(int n)
{
    int a = 1, b = 1, i;

    for (i = 2; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}
