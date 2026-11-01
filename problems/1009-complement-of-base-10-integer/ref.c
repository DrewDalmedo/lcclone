/* Reference solution: grow a mask of ones (1, 11, 111, ...) until it is at
 * least n, so that it has as many binary digits as n, and at least one.
 * n XOR mask swaps exactly those digits; for n = 0 the mask stays 1, which
 * gives 1. The mask never exceeds 2^30 - 1, so nothing overflows. */
int bitwiseComplement(int n)
{
    int mask = 1;

    while (mask < n)
        mask = mask * 2 + 1;
    return mask ^ n;
}
