/* Reference solution: a power of two is positive and has exactly one bit
 * set, and n & (n - 1) clears the lowest set bit. Testing n > 0 first also
 * keeps n - 1 from overflowing at INT_MIN. */
bool isPowerOfTwo(int n)
{
    return n > 0 && (n & (n - 1)) == 0;
}
