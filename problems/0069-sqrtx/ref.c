/* Reference solution: binary search for the largest r with r * r <= x.
 * r is at most 46340, since 46341^2 is past 2^31 - 1, and the square is
 * taken in 64 bits. */
int mySqrt(int x)
{
    int lo = 0, hi = x < 46340 ? x : 46340;

    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;

        if ((long long)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}
