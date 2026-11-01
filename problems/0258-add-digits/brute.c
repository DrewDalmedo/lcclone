/* Slow and obviously right: add up the digits, again and again, while the
 * value has two or more of them. */
int addDigits(int num)
{
    while (num >= 10) {
        int sum = 0;

        while (num > 0) {
            sum += num % 10;
            num /= 10;
        }
        num = sum;
    }
    return num;
}
