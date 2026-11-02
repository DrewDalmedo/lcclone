/* Reference solution: walk the rows, and in row i test the two diagonal
 * cells (i, i) and (i, n - 1 - i) by trial division up to the square root,
 * skipping any value no larger than the best prime found so far. */
static bool is_prime(int v)
{
    int d;

    if (v < 2)
        return false;
    for (d = 2; d * d <= v; d++)
        if (v % d == 0)
            return false;
    return true;
}

int diagonalPrime(int **nums, int numsSize, int *numsColSize)
{
    int n = numsSize, best = 0, i, k, v;

    (void)numsColSize;
    for (i = 0; i < n; i++)
        for (k = 0; k < 2; k++) {
            v = k == 0 ? nums[i][i] : nums[i][n - 1 - i];
            if (v > best && is_prime(v))
                best = v;
        }
    return best;
}
