/* Reference solution: take num's 32 bits as an unsigned number, which C
 * defines as num + 2^32 for a negative num, and peel off four bits at a
 * time, lowest first, writing the digits from the end of a buffer
 * backwards. The loop runs at least once, so 0 gives "0". */
char *toHex(int num)
{
    static const char digits[] = "0123456789abcdef";
    uint32_t u = (uint32_t)num;
    char buf[9], *out;
    int pos = 8;

    buf[pos] = '\0';
    do {
        buf[--pos] = digits[u & 15u];
        u >>= 4;
    } while (u != 0);
    out = malloc((size_t)(9 - pos));
    memcpy(out, buf + pos, (size_t)(9 - pos));
    return out;
}
