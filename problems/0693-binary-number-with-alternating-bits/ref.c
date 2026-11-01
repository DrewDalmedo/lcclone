/* Reference solution: n >> 1 puts each binary digit of n beside the digit
 * above it, so n ^ (n >> 1) has a 1 wherever two neighbours differ, and
 * its highest 1 is n's highest digit. The digits alternate exactly when
 * that XOR is a solid block of 1s, 2^L - 1, which is when adding 1 to it
 * leaves no bit in common with it. Unsigned arithmetic: for 1431655765
 * the block is 2^31 - 1, and adding 1 would overflow an int. */
bool hasAlternatingBits(int n)
{
    uint32_t x = (uint32_t)n ^ ((uint32_t)n >> 1);

    return (x & (x + 1u)) == 0;
}
