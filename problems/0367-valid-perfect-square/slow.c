/* Right, but too slow at full size: try every k from 1 up to num. It stops
 * early only on a square, so a number near 2^31 that is no square takes
 * about 2 * 10^9 steps. */
bool isPerfectSquare(int num)
{
    long long k;

    for (k = 1; k <= num; k++)
        if (k * k == num)
            return true;
    return false;
}
