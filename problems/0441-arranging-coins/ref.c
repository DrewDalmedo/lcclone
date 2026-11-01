/* Reference solution: binary search for the largest k with
 * k * (k + 1) / 2 <= n. The answer is below 65536, and the sums are
 * worked out in 64 bits, so nothing overflows. */
int arrangeCoins(int n)
{
    long long lo = 1, hi = 65536, mid; /* T(lo) <= n < T(hi) */

    while (hi - lo > 1) {
        mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return (int)lo;
}
