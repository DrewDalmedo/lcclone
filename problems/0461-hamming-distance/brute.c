/* Slow and obviously right: compare the last binary digits of x and y
 * (their remainders when divided by 2), then halve both, until both are
 * 0. */
int hammingDistance(int x, int y)
{
    int count = 0;

    while (x > 0 || y > 0) {
        if (x % 2 != y % 2)
            count++;
        x /= 2;
        y /= 2;
    }
    return count;
}
