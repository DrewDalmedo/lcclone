/* Slow and obviously right: the last move is one step or two, so count
 * the ways to reach each of the two stairs before it, by the same rule,
 * all the way down. The calls add up to about as many as the answer, so
 * this is far too slow for the largest n. */
int climbStairs(int n)
{
    if (n <= 1)
        return 1;
    return climbStairs(n - 1) + climbStairs(n - 2);
}
