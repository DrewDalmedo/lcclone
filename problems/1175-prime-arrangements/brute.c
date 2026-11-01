/* Slow and straight from the definition: fill the slots from left to
 * right, counting the ways while keeping track only of how many prime
 * cards are used up. A prime slot may take any card that is left;
 * another slot only a card whose number is not prime. Primes are found
 * by trying every divisor. */
static int is_prime(int x)
{
    int d;

    if (x < 2)
        return 0;
    for (d = 2; d < x; d++)
        if (x % d == 0)
            return 0;
    return 1;
}

int numPrimeArrangements(int n)
{
    const long long mod = 1000000007;
    /* ways[k]: the ways to fill the slots so far using k prime cards */
    long long ways[102] = {0}, next[102];
    int primes = 0, slot, k, x;

    for (x = 1; x <= n; x++)
        primes += is_prime(x);
    ways[0] = 1;
    for (slot = 1; slot <= n; slot++) {
        memset(next, 0, sizeof next);
        for (k = 0; k <= primes && k < slot; k++) {
            long long prime_left = primes - k;
            long long other_left = (n - primes) - (slot - 1 - k);

            if (other_left > 0)
                next[k] = (next[k] + ways[k] * other_left) % mod;
            if (is_prime(slot) && prime_left > 0)
                next[k + 1] = (next[k + 1] + ways[k] * prime_left) % mod;
        }
        memcpy(ways, next, sizeof ways);
    }
    return (int)ways[primes];
}
