/* One n with 0 <= n < 10^9. Small cases pick a shape at random: 0, a
 * number below 16, a power of two, one less or one more than a power of
 * two, or any number with 1 to 30 binary digits. Full-size ones always
 * have 30 binary digits (2^29 <= n < 10^9), the most an n can have. */
void gen(int small)
{
    long long n, low, high;
    int shape, width;

    if (!small) {
        put_int(rnd(1LL << 29, 999999999));
        return;
    }
    shape = (int)rnd(0, 5);
    width = (int)rnd(1, 30);
    low = 1LL << (width - 1);   /* the smallest number of `width` digits */
    high = 2 * low - 1;         /* the largest: `width` ones */
    if (high > 999999999)
        high = 999999999;
    switch (shape) {
    case 0:
        n = 0;
        break;
    case 1:
        n = rnd(1, 15);
        break;
    case 2:
        n = low;
        break;
    case 3:
        n = high;
        break;
    case 4:
        n = low + 1;
        break;
    default:
        n = rnd(low, high);
        break;
    }
    put_int(n);
}
