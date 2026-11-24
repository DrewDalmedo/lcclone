/* Right, but too slow at full size: test each diagonal value by trial
 * division by every number below it, with no square-root bound. On the
 * full-size case whose diagonals hold only large primes (10^6 to 4 * 10^6),
 * that is about 2 * 10^9 divisions. */
static bool is_prime(int v)
{
    int d;

    if (v < 2)
        return false;
    for (d = 2; d < v; d++)
        if (v % d == 0)
            return false;
    return true;
}

int diagonalPrime(int **nums, int numsSize, int *numsColSize)
{
    int n = numsSize, best = 0, i;

    (void)numsColSize;
    for (i = 0; i < n; i++) {
        if (is_prime(nums[i][i]) && nums[i][i] > best)
            best = nums[i][i];
        if (is_prime(nums[i][n - 1 - i]) && nums[i][n - 1 - i] > best)
            best = nums[i][n - 1 - i];
    }
    return best;
}
