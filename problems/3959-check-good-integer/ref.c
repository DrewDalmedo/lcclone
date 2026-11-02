/* Reference solution: take the digits off the end of n one at a time.
 * Each digit d adds d * d to Q and d to D, so add d * d - d to Q - D. */
bool checkGoodInteger(int n)
{
    int gap = 0, d;

    while (n > 0) {
        d = n % 10;
        gap += d * d - d;
        n /= 10;
    }
    return gap >= 50;
}
