/* Slow and obviously right, straight from the definition: the value to
 * write is num, or num + 2^32 when num is negative. Find the highest power
 * of 16 it reaches, then for each power from there down to 1, subtract it
 * as many times as it fits; that count is the digit. */
char *toHex(int num)
{
    long long v = num < 0 ? (long long)num + 4294967296LL : num, p = 1;
    char *out = malloc(9);
    int len = 0, d;

    while (p * 16 <= v)
        p *= 16;
    for (; p >= 1; p /= 16) {
        d = 0;
        while (v >= p) {
            v -= p;
            d++;
        }
        out[len++] = (char)(d < 10 ? '0' + d : 'a' + (d - 10));
    }
    out[len] = '\0';
    return out;
}
