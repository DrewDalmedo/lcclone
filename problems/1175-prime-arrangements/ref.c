/* Reference solution: count the primes up to n with a sieve; call it p.
 * The p prime cards must fill the p prime slots, in p! ways, and the
 * other cards the other slots, in (n - p)! ways. Multiply the two,
 * reducing modulo 10^9 + 7 at every step. */
int numPrimeArrangements(int n)
{
    const long long mod = 1000000007;
    char composite[101] = {0};
    long long ways = 1;
    int primes = 0, i, j;

    for (i = 2; i <= n; i++) {
        if (composite[i])
            continue;
        primes++;
        for (j = i * i; j <= n; j += i)
            composite[j] = 1;
    }
    for (i = 2; i <= primes; i++)
        ways = ways * i % mod;
    for (i = 2; i <= n - primes; i++)
        ways = ways * i % mod;
    return (int)ways;
}
