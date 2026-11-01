/* Reference solution: divisors come in pairs d and num / d, with d at
 * most the square root of num. Start the total at 1 (a divisor of every
 * num above 1), add both members of each pair for d from 2 while
 * d * d <= num, and the root only once when num is a square. num = 1 is
 * handled first: its total is 0. */
bool checkPerfectNumber(int num)
{
    long long sum = 1;
    int d;

    if (num == 1)
        return false;
    for (d = 2; d * d <= num; d++)
        if (num % d == 0) {
            sum += d;
            if (d != num / d)
                sum += num / d;
        }
    return sum == num;
}
