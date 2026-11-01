/* Reference solution: count each number's 1 bits by clearing the lowest
 * one until none is left, and look the count up in a bit mask of the
 * primes up to 19, the most 1 bits a number below 2^20 (and so up to
 * 10^6) can have. */
int countPrimeSetBits(int left, int right)
{
    const uint32_t primes = (1u << 2) | (1u << 3) | (1u << 5) | (1u << 7)
                          | (1u << 11) | (1u << 13) | (1u << 17)
                          | (1u << 19);
    int count = 0, ones, x;
    uint32_t v;

    for (x = left; x <= right; x++) {
        ones = 0;
        for (v = (uint32_t)x; v != 0; v &= v - 1)
            ones++;
        count += (int)((primes >> ones) & 1u);
    }
    return count;
}
