/* Reference solution: copy the highest 1 bit of num into every lower
 * position (OR-ing in num shifted right by 1, 2, 4, 8 and 16 places). That
 * gives the mask 2^L - 1 of num's L binary digits, and XOR with it flips
 * exactly those digits. Unsigned arithmetic, so nothing overflows. */
int findComplement(int num)
{
    uint32_t mask = (uint32_t)num;

    mask |= mask >> 1;
    mask |= mask >> 2;
    mask |= mask >> 4;
    mask |= mask >> 8;
    mask |= mask >> 16;
    return (int)(mask ^ (uint32_t)num);
}
