/* Slow and obviously right: spell each number out in base 2, digit by
 * digit, and count the 1s; divide by taking p away from q as many times
 * as it fits. */
static int count_ones(int x)
{
    char digits[40];
    int len = 0, count = 0, i;

    while (x > 0) {
        digits[len++] = (char)('0' + x % 2);
        x /= 2;
    }
    for (i = 0; i < len; i++)
        if (digits[i] == '1')
            count++;
    return count;
}

int calculateScore(int a, int b)
{
    int p = count_ones(a), q = count_ones(b), times = 0;

    if (p >= q)
        return p * q;
    while (q >= p) {
        q -= p;
        times++;
    }
    return times;
}
