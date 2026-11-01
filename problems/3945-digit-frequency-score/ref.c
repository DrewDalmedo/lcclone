/* Reference solution: tally the decimal digits of n by taking them off the
 * end with % 10, then add d * count[d] over the ten digits. (That total is
 * just the sum of the digits, since each copy of d adds d once.) */
int digitFrequencyScore(int n)
{
    int count[10] = {0}, total = 0, d;

    for (; n > 0; n /= 10)
        count[n % 10]++;
    for (d = 0; d < 10; d++)
        total += d * count[d];
    return total;
}
