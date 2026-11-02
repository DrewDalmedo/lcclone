/* A random square grid. Small cases have sides 1 to 6 and mix small numbers
 * (many primes and 1s) with traps: squares of primes, products of two primes
 * near 2000, numbers that fool weak probabilistic tests, 4 * 10^6 and large
 * primes; about one in seven puts no prime on the diagonals. Full-size cases
 * have side 299 or 300 and, by seed, fill the diagonals with large primes,
 * with large composites only (answer 0), with large composites but one prime
 * in the bottom-left corner, or with random values. */
static const int big_primes[] = {
    3999971, 3999949, 3999929, 3999917, 3999901, 3999893, 3999001,
    2000003, 1999993, 1000003
};
static const int near_2000[] = {
    1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997,
    1999
};
static const int traps[] = {
    1, 4, 9, 25, 49, 121, 561, 1105, 1729, 2047, 8911, 1373653, 3988009,
    3996001, 3999999, 4000000
};

#define COUNT(a) ((int)(sizeof (a) / sizeof (a)[0]))

static int pick(const int *a, int n)
{
    return a[rnd(0, n - 1)];
}

/* A number that is not prime; small ones stay below 100 when asked. */
static int not_prime(int small)
{
    int kind = (int)rnd(0, 2), a, b;

    if (kind == 0)
        return pick(traps, COUNT(traps));
    if (kind == 1 && small) {
        a = (int)rnd(2, 9);
        b = (int)rnd(2, 9);
        return a * b;
    }
    a = pick(near_2000, COUNT(near_2000));
    b = pick(near_2000, COUNT(near_2000));
    return a * b;
}

static int small_value(void)
{
    int kind = (int)rnd(0, 99);

    if (kind < 45)
        return (int)rnd(1, 20);
    if (kind < 57)
        return pick(traps, COUNT(traps));
    if (kind < 72)
        return pick(big_primes, COUNT(big_primes));
    if (kind < 80)
        return not_prime(0);
    return (int)rnd(1, 4000000);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : (int)rnd(299, 300);
    int shape = small ? (rnd_chance(15) ? 1 : 3) : (int)rnd(0, 3);
    int *a = malloc((size_t)n * n * sizeof *a);
    int r, c, diag;

    for (r = 0; r < n; r++)
        for (c = 0; c < n; c++) {
            diag = r == c || r + c == n - 1;
            if (!diag)
                a[r * n + c] = small ? small_value() : (int)rnd(1, 4000000);
            else if (shape == 0)
                a[r * n + c] = pick(big_primes, COUNT(big_primes));
            else if (shape == 1 || shape == 2)
                a[r * n + c] = not_prime(small);
            else
                a[r * n + c] = small ? small_value() : (int)rnd(1, 4000000);
        }
    if (shape == 2)
        a[(n - 1) * n] = pick(big_primes, COUNT(big_primes));
    put_int_grid(a, n, n);
    free(a);
}
