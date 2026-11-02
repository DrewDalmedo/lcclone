/* Slow and obviously right, by other means: the square goes through
 * printf's %X, and the cube is written from its top digit down, taking
 * out the largest power of 36 as many times as it fits. */
char *concatHex36(int n)
{
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    long long cube = (long long)n * n * n, p = 1;
    char *out = malloc(32);
    int len = sprintf(out, "%X", (unsigned)(n * n)), d;

    while (p * 36 <= cube)
        p *= 36;
    for (; p >= 1; p /= 36) {
        d = 0;
        while (cube >= p) {
            cube -= p;
            d++;
        }
        out[len++] = digits[d];
    }
    out[len] = '\0';
    return out;
}
