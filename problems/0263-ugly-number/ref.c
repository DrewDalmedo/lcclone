/* Reference solution: a positive n is ugly exactly when dividing out every
 * factor 2, 3 and 5 leaves 1. Zero and negatives are turned away first
 * (zero would stay divisible by 2 forever). */
bool isUgly(int n)
{
    static const int primes[3] = {2, 3, 5};
    int i;

    if (n <= 0)
        return false;
    for (i = 0; i < 3; i++)
        while (n % primes[i] == 0)
            n /= primes[i];
    return n == 1;
}
