/* Picks a shape so that true answers and the near misses come up often:
 * the square of a prime, a higher power of a prime (p^3, p^4, ...), the
 * square of a composite, a prime, a product of two primes, a neighbour of
 * a prime square, or any number. Small cases stay at or below 200,
 * full-size ones go up to 10^4. */
static int is_prime(int x)
{
    int d;

    if (x < 2)
        return 0;
    for (d = 2; d * d <= x; d++)
        if (x % d == 0)
            return 0;
    return 1;
}

/* A random prime p with p * p <= top when square is set, p <= top else. */
static int pick_prime(int top, int square)
{
    int p;

    do
        p = (int)rnd(2, square ? 100 : top);
    while (!is_prime(p) || (square ? p * p > top : p > top));
    return p;
}

void gen(int small)
{
    int top = small ? 200 : 10000, shape = (int)rnd(0, 6), n, p, q, m;

    if (shape == 0) {
        p = pick_prime(top, 1);
        n = p * p;
    } else if (shape == 1) {
        p = pick_prime(small ? 5 : 21, 0);
        n = p * p * p;
        while (n * p <= top && rnd_chance(50))
            n *= p;
    } else if (shape == 2) {
        do
            m = (int)rnd(4, 100);
        while (is_prime(m) || m * m > top);
        n = m * m;
    } else if (shape == 3) {
        n = pick_prime(top, 0);
    } else if (shape == 4) {
        p = pick_prime(top / 2, 0);
        do
            q = pick_prime(top / p, 0);
        while (q == p);
        n = p * q;
    } else if (shape == 5) {
        p = pick_prime(top, 1);
        n = p * p;
        m = rnd_chance(50) ? 1 : -1;
        if (n + m <= top)
            n += m;
        else
            n -= 1;
    } else {
        n = (int)rnd(1, top);
    }
    put_int(n);
}
