/* Reference solution: a power of four is positive, has exactly one bit set
 * (n & (n - 1) clears the lowest one), and that bit is at an even position,
 * which the mask 0x55555555 (bits 0, 2, ..., 30) picks out. Testing n > 0
 * first keeps n - 1 from overflowing at INT_MIN. */
bool isPowerOfFour(int n)
{
    return n > 0 && (n & (n - 1)) == 0 && (n & 0x55555555) != 0;
}
