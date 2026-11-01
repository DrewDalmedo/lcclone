/* Multiplies n by factors drawn from 2, 3 and 5 while the product stays at
 * most limit, stopping early with a chance of stop in 100 at each factor.
 * With stop = 0 it ends above limit / 2, as close to limit as it gets. */
static long long grow(long long n, long long limit, int stop)
{
    static const int f[3] = {2, 3, 5};

    while (n * 2 <= limit && !rnd_chance(stop)) {
        int p = f[rnd(0, 2)];

        if (n * p <= limit)
            n *= p;
    }
    return n;
}

/* The seed picks a shape: an ugly number, an ugly number times a prime
 * larger than 5, minus an ugly number, or any number. Small cases stay
 * near 1000 or below, so 0, 1 and small negatives come up often; full-size
 * ones grow as large as an int allows, which also makes the most divisions
 * for a solution that divides the factors out, and reach -2^31. */
void gen(int small)
{
    static const long long big[6] = {7, 11, 13, 97, 65537, 1073741789};
    const long long top = 2147483647;
    int shape = (int)rnd(0, 3);

    if (small) {
        if (shape == 0)
            put_int(grow(1, 1000, 25));
        else if (shape == 1)
            put_int(grow(big[rnd(0, 2)], 1000, 25));
        else if (shape == 2)
            put_int(-grow(1, 1000, 25));
        else
            put_int(rnd(-20, 100));
        return;
    }
    if (shape == 0)
        put_int(grow(1, top, 0));
    else if (shape == 1)
        put_int(grow(big[rnd(0, 5)], top, 0));
    else if (shape == 2)
        put_int(-grow(1, top + 1, 0));
    else
        put_int(rnd(-top - 1, top));
}
