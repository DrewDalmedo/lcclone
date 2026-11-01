/* Reference solution: walk the divisors of n in pairs (d and n / d, for d
 * up to the square root of n), adding the square of the element at each
 * of those 1-based positions once. */
int sumOfSquares(int *nums, int numsSize)
{
    int n = numsSize, sum = 0, d, v;

    for (d = 1; d * d <= n; d++) {
        if (n % d != 0)
            continue;
        v = nums[d - 1];
        sum += v * v;
        if (d != n / d) {
            v = nums[n / d - 1];
            sum += v * v;
        }
    }
    return sum;
}
