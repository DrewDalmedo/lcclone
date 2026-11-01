/* Slow and obviously right: compare n with each of 4^0, 4^1, ..., 4^15,
 * every power of four that fits in an int, worked out in 64 bits. */
bool isPowerOfFour(int n)
{
    long long p = 1;
    int k;

    for (k = 0; k <= 15; k++) {
        if (p == n)
            return true;
        p *= 4;
    }
    return false;
}
