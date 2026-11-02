/* Reference solution: count each value in a table indexed by value
 * (0..100), then test every nonzero count for primality by trial
 * division up to its square root. */
static bool isPrime(int f)
{
    int d;

    if (f < 2)
        return false;
    for (d = 2; d * d <= f; d++)
        if (f % d == 0)
            return false;
    return true;
}

bool checkPrimeFrequency(int *nums, int numsSize)
{
    int count[101] = {0}, i;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (i = 0; i <= 100; i++)
        if (isPrime(count[i]))
            return true;
    return false;
}
