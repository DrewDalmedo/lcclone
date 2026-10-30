/* Reference solution: move each bit to its mirrored position. */
uint32_t reverseBits(uint32_t n)
{
    uint32_t r = 0;
    int i;

    for (i = 0; i < 32; i++) {
        r = (r << 1) | (n & 1u);
        n >>= 1;
    }
    return r;
}
