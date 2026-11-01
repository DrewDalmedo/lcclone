/* Slow and obviously right: write each number out with sprintf, skip it if
 * it has an odd count of digits, and add up the digits of its two halves. */
int countSymmetricIntegers(int low, int high)
{
    char buf[16];
    int count = 0, x, len, i, left, right;

    for (x = low; x <= high; x++) {
        len = sprintf(buf, "%d", x);
        if (len % 2 != 0)
            continue;
        left = right = 0;
        for (i = 0; i < len / 2; i++)
            left += buf[i] - '0';
        for (i = len / 2; i < len; i++)
            right += buf[i] - '0';
        if (left == right)
            count++;
    }
    return count;
}
