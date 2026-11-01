/* Slow and obviously right: for every rung x, add up both totals rung by
 * rung and compare them. */
int pivotInteger(int n)
{
    int x, k, low, high;

    for (x = 1; x <= n; x++) {
        low = 0;
        for (k = 1; k <= x; k++)
            low += k;
        high = 0;
        for (k = x; k <= n; k++)
            high += k;
        if (low == high)
            return x;
    }
    return -1;
}
