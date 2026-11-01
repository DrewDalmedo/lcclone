/* Slow and obviously right: count r up from 0 while (r + 1)^2 still fits
 * under x, in 64-bit arithmetic. */
int mySqrt(int x)
{
    long long r = 0;

    while ((r + 1) * (r + 1) <= x)
        r++;
    return (int)r;
}
