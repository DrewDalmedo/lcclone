/* Slow and obviously right: try all 100 recodings (x, y), work out the
 * number each one gives, and keep the largest and the smallest. */
int minMaxDifference(int num)
{
    int digits[12], len = 0, x, y, i, d, v, hi = 0, lo = 0, t = num;

    while (t > 0) {
        digits[len++] = t % 10;
        t /= 10;
    }
    for (x = 0; x <= 9; x++)
        for (y = 0; y <= 9; y++) {
            v = 0;
            for (i = len - 1; i >= 0; i--) {
                d = digits[i] == x ? y : digits[i];
                v = v * 10 + d;
            }
            if ((x == 0 && y == 0) || v > hi)
                hi = v;
            if ((x == 0 && y == 0) || v < lo)
                lo = v;
        }
    return hi - lo;
}
