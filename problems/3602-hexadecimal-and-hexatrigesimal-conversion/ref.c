/* Reference solution: write each number by repeated division, which
 * yields its digits from the units up, then reverse them into place. */
static int appendBase(char *out, int len, long long v, int base)
{
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char tmp[16];
    int k = 0;

    while (v > 0) {
        tmp[k++] = digits[v % base];
        v /= base;
    }
    while (k > 0)
        out[len++] = tmp[--k];
    return len;
}

char *concatHex36(int n)
{
    long long sq = (long long)n * n, cube = sq * n;
    char *out = malloc(16);
    int len = 0;

    len = appendBase(out, len, sq, 16);
    len = appendBase(out, len, cube, 36);
    out[len] = '\0';
    return out;
}
