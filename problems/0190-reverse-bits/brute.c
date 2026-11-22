/* Slow and obviously right: write out the 32 bits as text, highest
 * first, and read the text back from its other end. */
uint32_t reverseBits(uint32_t n)
{
    char bits[32];
    uint32_t r = 0;
    int i;

    for (i = 0; i < 32; i++)
        bits[i] = n & (1u << (31 - i)) ? '1' : '0';
    for (i = 31; i >= 0; i--)
        r = r * 2 + (uint32_t)(bits[i] == '1');
    return r;
}
