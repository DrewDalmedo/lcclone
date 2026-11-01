/* Slow and obviously right: try every d from 1 to num - 1 and add up the
 * ones that divide num. */
bool checkPerfectNumber(int num)
{
    long long sum = 0;
    int d;

    for (d = 1; d < num; d++)
        if (num % d == 0)
            sum += d;
    return sum == num;
}
