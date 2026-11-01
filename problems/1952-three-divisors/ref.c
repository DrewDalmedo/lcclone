/* Reference solution: the divisors of n pair up as d and n / d, so the
 * count is odd only when n is a square r * r. It is exactly three when r is
 * a prime, since the divisors are then 1, r and n. */
bool isThree(int n)
{
    int r = 1, d;

    while ((r + 1) * (r + 1) <= n)
        r++;
    if (r * r != n || r < 2)
        return false;
    for (d = 2; d * d <= r; d++)
        if (r % d == 0)
            return false;
    return true;
}
