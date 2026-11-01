/* Reference solution: peel off the digits from the last one with % 10,
 * filling a buffer from its right end, and put a '.' in front of every
 * third digit that still has more digits before it. 13 characters (10
 * digits and 3 dots) are the most it can take. */
char *thousandSeparator(int n)
{
    char buf[16];
    int pos = 15, count = 0;
    char *out;

    buf[pos] = '\0';
    do {
        if (count > 0 && count % 3 == 0)
            buf[--pos] = '.';
        buf[--pos] = (char)('0' + n % 10);
        n /= 10;
        count++;
    } while (n > 0);
    out = malloc((size_t)(16 - pos));
    memcpy(out, buf + pos, (size_t)(16 - pos));
    return out;
}
