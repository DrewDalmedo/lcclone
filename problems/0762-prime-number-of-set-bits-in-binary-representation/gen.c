/* Two ints, left <= right <= 10^6. Small cases are short ranges, up to 21
 * numbers, that start at a small number, just below a power of two (where
 * the count of 1s climbs and then drops to one), just below 2^20 - 1 - 2^j
 * for j = 16 to 19 (counts of 1s up to 19, the most there can be), or
 * near 10^6. Full-size cases are as wide as allowed, right - left = 10^4,
 * and start at 1, end at 10^6, or lie anywhere in between. */
void gen(int small)
{
    long long left, right, width, off;
    int where, k;

    if (small) {
        where = (int)rnd(0, 3);
        width = rnd(0, 20);
        off = rnd(0, 20);
        if (where == 0) {
            left = rnd(1, 64);
        } else if (where == 1) {
            k = (int)rnd(1, 19);
            left = (1LL << k) - off;
        } else if (where == 2) {
            k = (int)rnd(16, 19);
            left = (1LL << 20) - 1 - (1LL << k) - off;
        } else {
            left = 1000000 - off;
        }
        if (left < 1)
            left = 1;
        right = left + width;
        if (right > 1000000)
            right = 1000000;
    } else {
        where = (int)rnd(0, 2);
        width = 10000;
        if (where == 0)
            left = 1;
        else if (where == 1)
            left = 1000000 - width;
        else
            left = rnd(1, 1000000 - width);
        right = left + width;
    }
    put_int(left);
    put_int(right);
}
