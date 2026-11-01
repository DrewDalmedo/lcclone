/* x at, just below or just above a perfect square, or anywhere. Small
 * cases keep the root at most 44 (x below 2025), with tiny values often.
 * Full-size cases keep the root from 32768 to 46340 (x from 2^30 - 1 up
 * to 2^31 - 1), where counting up takes the most steps and squaring a
 * guess in 32 bits overflows; the seed picks the shape, and the last one
 * is within 10^5 of 2^31 - 1, around 46340^2 = 2147395600. */
void gen(int small)
{
    long long rmin = small ? 1 : 32768, rmax = small ? 44 : 46340, r, x;

    switch ((int)rnd(0, 4)) {
    case 0: /* a perfect square */
        r = rnd(small ? 0 : rmin, rmax);
        x = r * r;
        break;
    case 1: /* one below a perfect square */
        r = rnd(rmin, rmax);
        x = r * r - 1;
        break;
    case 2: /* strictly between r^2 and (r + 1)^2 */
        r = rnd(rmin, rmax);
        x = r * r + rnd(1, 2 * r);
        break;
    case 3:
        x = small ? rnd(0, 2024) : rnd(1073741824, 2147483647);
        break;
    default:
        x = small ? rnd(0, 10) : 2147483647 - rnd(0, 100000);
    }
    if (x > 2147483647)
        x = 2147483647;
    put_int(x);
}
