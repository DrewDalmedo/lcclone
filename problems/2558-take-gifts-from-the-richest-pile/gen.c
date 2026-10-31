/* Piles of mixed sizes: small ones, huge ones, and perfect squares (or one
 * less) up to 10^9, where a careless square root goes wrong. Small cases
 * have few piles and a k that often runs every pile down to 1. */
static int pile(void)
{
    long long s;

    switch ((int)rnd(0, 3)) {
    case 0:
        return (int)rnd(1, 20);
    case 1:
        return (int)rnd(1, 100000);
    case 2:
        return (int)rnd(1, 1000000000);
    default:
        s = rnd(1, 31622);
        return (int)(s * s - (s > 1 ? rnd(0, 1) : 0));
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 1000, i;
    int k = small ? (int)rnd(1, 15) : (int)rnd(1, 1000);
    int gifts[1000];

    for (i = 0; i < n; i++)
        gifts[i] = pile();
    put_ints(gifts, n);
    put_int(k);
}
