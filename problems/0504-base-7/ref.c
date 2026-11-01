/* Reference solution: divide the magnitude of num by 7 again and again,
 * writing each remainder as a digit from the end of a buffer backwards,
 * then put a minus sign in front for a negative num. The loop runs at
 * least once, so 0 gives "0". At most 9 digits and a sign, since
 * |num| <= 10^7 < 7^9. */
char *convertToBase7(int num)
{
    char buf[12], *out;
    int v = num < 0 ? -num : num, pos = 11;

    buf[pos] = '\0';
    do {
        buf[--pos] = (char)('0' + v % 7);
        v /= 7;
    } while (v != 0);
    if (num < 0)
        buf[--pos] = '-';
    out = malloc((size_t)(12 - pos));
    memcpy(out, buf + pos, (size_t)(12 - pos));
    return out;
}
