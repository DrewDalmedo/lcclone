/* Reference solution: count the balls of each box in a table indexed by
 * digit sum (at most 45, from 99999) and return the largest count. */
int countBalls(int lowLimit, int highLimit)
{
    int count[46] = {0};
    int best = 0, b, x, sum;

    for (b = lowLimit; b <= highLimit; b++) {
        sum = 0;
        for (x = b; x > 0; x /= 10)
            sum += x % 10;
        if (++count[sum] > best)
            best = count[sum];
    }
    return best;
}
