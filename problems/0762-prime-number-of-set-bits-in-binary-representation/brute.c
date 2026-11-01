/* Slow and obviously right: for each ticket, count the 1s among the
 * remainders of halving it down to 0, then test that count for primality
 * by trying every divisor between 2 and the count itself. */
static bool is_prime(int p)
{
    int d;

    if (p < 2)
        return false;
    for (d = 2; d < p; d++)
        if (p % d == 0)
            return false;
    return true;
}

int countPrimeSetBits(int left, int right)
{
    int count = 0, ones, x, v;

    for (x = left; x <= right; x++) {
        ones = 0;
        for (v = x; v > 0; v /= 2)
            ones += v % 2;
        if (is_prime(ones))
            count++;
    }
    return count;
}
