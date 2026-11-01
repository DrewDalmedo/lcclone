/* Slow and obviously right: add up the digits of every number from 1 to
 * num and count the even totals. */
int countEven(int num)
{
    int count = 0, x, y, sum;

    for (x = 1; x <= num; x++) {
        sum = 0;
        for (y = x; y > 0; y /= 10)
            sum += y % 10;
        if (sum % 2 == 0)
            count++;
    }
    return count;
}
