/* Reference solution: sort the distinct values from largest to smallest and
 * try every pair with Euclid's gcd, as (a / g) * (b / g) in 64 bits. Every
 * pair has strength at least 1, and at most a * b, so the scan stops as soon
 * as a * b can no longer beat the best strength found. */
static int strength_desc(const void *p, const void *q)
{
    int x = *(const int *)p, y = *(const int *)q;

    return (x < y) - (x > y);
}

static long long strength_gcd(long long a, long long b)
{
    while (b != 0) {
        long long t = a % b;

        a = b;
        b = t;
    }
    return a;
}

long long maxPairStrength(int *nums, int numsSize)
{
    int *v = malloc((size_t)numsSize * sizeof *v), n = 0, i, j;
    long long best = 1, a, b, g;

    memcpy(v, nums, (size_t)numsSize * sizeof *v);
    qsort(v, (size_t)numsSize, sizeof *v, strength_desc);
    for (i = 0; i < numsSize; i++)
        if (n == 0 || v[n - 1] != v[i])
            v[n++] = v[i];
    for (i = 0; i + 1 < n && (long long)v[i] * v[i + 1] > best; i++)
        for (j = i + 1; j < n; j++) {
            a = v[i];
            b = v[j];
            if (a * b <= best)
                break;
            g = strength_gcd(a, b);
            if ((a / g) * (b / g) > best)
                best = (a / g) * (b / g);
        }
    free(v);
    return best;
}
