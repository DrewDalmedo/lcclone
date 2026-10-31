/* Slow and obviously right: every pair of different indices, with the gcd
 * found by trying every divisor from the smaller value down, and the
 * strength worked out as written: a * b / gcd^2. */
long long maxPairStrength(int *nums, int numsSize)
{
    long long best = 0, a, b, g, s;
    int i, j;

    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsSize; j++) {
            if (i == j)
                continue;
            a = nums[i];
            b = nums[j];
            g = a < b ? a : b;
            while (a % g != 0 || b % g != 0)
                g--;
            s = a * b / (g * g);
            if (s > best)
                best = s;
        }
    return best;
}
