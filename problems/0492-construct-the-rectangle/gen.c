/* A perfect square k^2; a near square k * (k + d) for a small d, where
 * the root's floor or ceiling divides area; a prime, whose only answer is
 * [area, 1]; a product of two primes, close to each other or far apart; a
 * number with many divisors; a number near the edges; or any number.
 * Small cases keep area at most 200; full-size ones reach 10^7 and lean to
 * the top of the range, where a walk from the root down to W = 1 is
 * longest. */
static int is_prime(long long n)
{
    long long d;

    if (n < 2)
        return 0;
    for (d = 2; d * d <= n; d++)
        if (n % d == 0)
            return 0;
    return 1;
}

/* The largest prime that is at most n (n >= 2). */
static long long prime_below(long long n)
{
    while (!is_prime(n))
        n--;
    return n;
}

void gen(int small)
{
    static const long long edge[] = {1, 9998244, 9999991, 10000000};
    long long top = small ? 200 : 10000000, kmax = small ? 14 : 3162;
    long long area, k, d, p, q;

    switch ((int)rnd(0, 6)) {
    case 0:
        k = rnd(small ? 1 : kmax / 2, kmax);
        area = k * k;
        break;
    case 1:
        d = rnd(1, 3);
        k = rnd(small ? 1 : kmax / 2, kmax);
        area = k * (k + d);
        if (area > top)
            area = (k - d) * k;
        break;
    case 2:
        p = rnd(small ? 2 : top / 2, top);
        area = prime_below(p);
        break;
    case 3:
        if (rnd_chance(50)) {
            p = prime_below(rnd(small ? 2 : kmax * 3 / 4, kmax));
            q = prime_below(rnd(p, top / p));
        } else {
            p = prime_below(rnd(2, small ? 5 : 100));
            q = rnd(top / (4 * p), top / p);
            q = prime_below(q < 2 ? 2 : q);
        }
        area = p * q;
        break;
    case 4:
        k = small ? rnd(1, 16) : rnd(1, top / 720);
        area = (small ? 12 : 720) * k;
        break;
    case 5:
        if (small)
            area = rnd(1, 12);
        else if (rnd_chance(40))
            area = edge[rnd(0, 3)];
        else
            area = rnd(top - 100000, top);
        break;
    default:
        area = rnd(1, top);
    }
    put_int(area);
}
