/* Reference solution: binary search for m, the number of full helpings
 * (the largest m with m * (m + 1) / 2 <= candies; m < 44722). Seat i gets
 * the full helpings i + 1, i + 1 + n, i + 1 + 2n, ... up to m: c of them,
 * adding up to c * (i + 1) + n * c * (c - 1) / 2. What is left over, if
 * anything, goes to seat m mod n, whose turn comes next. All in 64 bits. */
int *distributeCandies(int candies, int num_people, int *returnSize)
{
    long long n = num_people, lo = 0, hi = 44722, mid, c, i, left;
    int *out = malloc((size_t)num_people * sizeof *out);

    while (hi - lo > 1) { /* lo*(lo+1)/2 <= candies < hi*(hi+1)/2 */
        mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 <= candies)
            lo = mid;
        else
            hi = mid;
    }
    for (i = 0; i < n; i++) {
        c = i + 1 <= lo ? (lo - (i + 1)) / n + 1 : 0;
        out[i] = (int)(c * (i + 1) + n * c * (c - 1) / 2);
    }
    left = candies - lo * (lo + 1) / 2;
    if (left > 0)
        out[lo % n] += (int)left;
    *returnSize = num_people;
    return out;
}
