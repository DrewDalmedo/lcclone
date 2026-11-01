/* Reference solution: the numbers 0, 1, 2, ... pair up as (0, 1), (2, 3),
 * ..., and the two numbers of a pair differ only in the last digit, by 1,
 * so exactly one of them has an even digit sum. Counting the pairs up to
 * num and leaving out 0 gives num / 2 when num's own digit sum is even,
 * and (num - 1) / 2 when it is odd (for odd num the two are the same).
 * O(log num) time. */
int countEven(int num)
{
    int sum = 0, x;

    for (x = num; x > 0; x /= 10)
        sum += x % 10;
    return sum % 2 == 0 ? num / 2 : (num - 1) / 2;
}
