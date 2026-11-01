/* Reference solution: the answer has as many binary digits as n, all of
 * them 1. Grow a mask of ones until it covers every bit of n. */
int smallestNumber(int n)
{
    int mask = 1;

    while (mask < n)
        mask = mask * 2 + 1;
    return mask;
}
