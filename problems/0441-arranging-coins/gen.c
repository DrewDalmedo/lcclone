/* n at, just below or just above a sum T(k) = k * (k + 1) / 2, or
 * anywhere; the seed picks the shape. Small cases keep n at most 1900 (k
 * at most 61), with tiny values often. Full-size ones keep k from 46341
 * to 65535 (n from about 2^30 up to 2^31 - 1), where filling the rows
 * one by one takes the most rounds and k * (k + 1) overflows a 32-bit
 * int; the last shape is within 10^5 of 2^31 - 1. */
static long long tri(long long k)
{
    return k * (k + 1) / 2;
}

void gen(int small)
{
    long long kmin = small ? 2 : 46341, kmax = small ? 60 : 65535, k, n;

    switch ((int)rnd(0, 4)) {
    case 0: /* every row full */
        k = rnd(small ? 1 : kmin, kmax);
        n = tri(k);
        break;
    case 1: /* one coin short of filling row k */
        k = rnd(kmin, kmax);
        n = tri(k) - 1;
        break;
    case 2: /* row k + 1 started but not full */
        k = rnd(kmin, kmax);
        n = tri(k) + rnd(1, k);
        break;
    case 3:
        n = small ? rnd(1, 1900) : rnd(1073741824, 2147483647);
        break;
    default:
        n = small ? rnd(1, 10) : 2147483647 - rnd(0, 100000);
    }
    if (n > 2147483647)
        n = 2147483647;
    put_int(n);
}
