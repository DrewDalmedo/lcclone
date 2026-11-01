/* Slow and obviously right, as the statement says: halve n until it is 0,
 * and compare each remainder with the one before it. */
bool hasAlternatingBits(int n)
{
    int prev = n % 2;

    n /= 2;
    while (n > 0) {
        if (n % 2 == prev)
            return false;
        prev = n % 2;
        n /= 2;
    }
    return true;
}
