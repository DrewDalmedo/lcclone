/* Reference solution: keep the bits at even positions with the mask
 * 0x55555555 and those at odd positions with 0x2AAAAAAA, and count the
 * 1s left in each, clearing the lowest 1 bit at every step. */
static int ones(unsigned v)
{
    int count = 0;

    while (v) {
        v &= v - 1;
        count++;
    }
    return count;
}

int *evenOddBit(int n, int *returnSize)
{
    int *out = malloc(2 * sizeof *out);

    out[0] = ones((unsigned)n & 0x55555555u);
    out[1] = ones((unsigned)n & 0x2AAAAAAAu);
    *returnSize = 2;
    return out;
}
