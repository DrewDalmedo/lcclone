/* Reference solution: divide and conquer on the bits. Add neighbouring
 * bits in pairs, then the pair counts in groups of four bits, then those
 * in bytes, and finally add up the four byte counts with one
 * multiplication. Unsigned arithmetic, so nothing overflows. */
int hammingWeight(int n)
{
    uint32_t x = (uint32_t)n;

    x = x - ((x >> 1) & 0x55555555u);
    x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
    x = (x + (x >> 4)) & 0x0F0F0F0Fu;
    return (int)((x * 0x01010101u) >> 24);
}
