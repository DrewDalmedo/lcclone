/* Reference solution: try n, n + 1, ... in turn, multiplying the digits of
 * each with arithmetic on a copy. A multiple of 10 has a 0 digit, so at
 * most ten numbers are tried. */
int smallestNumber(int n, int t)
{
    int m, x, prod;

    for (m = n;; m++) {
        prod = 1;
        for (x = m; x > 0; x /= 10)
            prod *= x % 10;
        if (prod % t == 0)
            return m;
    }
}
