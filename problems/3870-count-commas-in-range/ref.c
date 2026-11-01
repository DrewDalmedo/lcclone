/* Reference solution: a number of d digits carries (d - 1) / 3 commas,
 * so walk through the bands of numbers with 1, 2, 3, ... digits and add
 * up how many numbers of each band lie in 1..n, times its commas. */
int countCommas(int n)
{
    long long lo = 1, hi, total = 0;
    int digits = 1;

    while (lo <= n) {
        hi = lo * 10 - 1;
        if (hi > n)
            hi = n;
        total += (hi - lo + 1) * ((digits - 1) / 3);
        lo *= 10;
        digits++;
    }
    return (int)total;
}
