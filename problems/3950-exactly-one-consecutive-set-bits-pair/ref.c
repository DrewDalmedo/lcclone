/* Reference solution: n & (n >> 1) has a 1 exactly at the low bit of each
 * pair of neighbouring 1s, overlapping pairs included. There is exactly
 * one pair when that value is not 0 and has a single 1 bit. */
bool consecutiveSetBits(int n)
{
    int pairs = n & (n >> 1);

    return pairs != 0 && (pairs & (pairs - 1)) == 0;
}
