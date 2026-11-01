/* Slow and obviously right: try every string of n binary digits with as
 * many '1's as s, as a number from the largest down, and return the first
 * odd one. Only for short strings (n up to 20). */
char *maximumOddBinaryNumber(char *s)
{
    int n = (int)strlen(s), ones = 0, bits, i;
    long long v, x;
    char *out = malloc((size_t)n + 1);

    for (i = 0; i < n; i++)
        if (s[i] == '1')
            ones++;
    for (v = (1LL << n) - 1; v >= 0; v--) {
        bits = 0;
        for (x = v; x != 0; x /= 2)
            bits += (int)(x % 2);
        if (bits == ones && v % 2 == 1)
            break;
    }
    for (i = 0; i < n; i++)
        out[i] = (char)('0' + ((v >> (n - 1 - i)) & 1));
    out[n] = '\0';
    return out;
}
