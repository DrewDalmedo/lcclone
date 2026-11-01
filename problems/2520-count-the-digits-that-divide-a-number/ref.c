/* Reference solution: peel the digits off a copy of num with % 10 and
 * / 10, and count those that num itself is a multiple of. */
int countDigits(int num)
{
    int count = 0, x;

    for (x = num; x > 0; x /= 10)
        if (num % (x % 10) == 0)
            count++;
    return count;
}
