/* Slow and obviously right: try every integer with at most n digits, from
 * the largest, 10^n - 1, down to 0, and return the first whose digits add
 * up to s. */
int largestInteger(int n, int s)
{
    int top = 1, x, y, sum, i;

    for (i = 0; i < n; i++)
        top *= 10;
    for (x = top - 1; x >= 0; x--) {
        sum = 0;
        for (y = x; y > 0; y /= 10)
            sum += y % 10;
        if (sum == s)
            return x;
    }
    return -1;
}
