/* Reference solution: fill a[2..n] from left to right with the two rules
 * (an even index copies index j / 2, an odd one adds the two indices
 * around half of it), keeping the largest value seen. a[1] exists only
 * when n >= 1. */
int getMaximumGenerated(int n)
{
    int a[101], best, j;

    a[0] = 0;
    if (n == 0)
        return 0;
    a[1] = 1;
    best = 1;
    for (j = 2; j <= n; j++) {
        if (j % 2 == 0)
            a[j] = a[j / 2];
        else
            a[j] = a[j / 2] + a[j / 2 + 1];
        if (a[j] > best)
            best = a[j];
    }
    return best;
}
