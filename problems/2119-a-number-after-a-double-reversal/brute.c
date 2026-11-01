/* Slow and obviously right: flip the digits twice, as the statement says,
 * and compare. */
static int flip(int x)
{
    int r = 0;

    while (x > 0) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

bool isSameAfterReversals(int num)
{
    return flip(flip(num)) == num;
}
