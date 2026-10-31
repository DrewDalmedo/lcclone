/* Slow and obviously right: double p from 1, in 64 bits, until it reaches
 * n or passes it, and see whether it landed on n. */
bool isPowerOfTwo(int n)
{
    long long p = 1;

    while (p < n)
        p *= 2;
    return p == n;
}
