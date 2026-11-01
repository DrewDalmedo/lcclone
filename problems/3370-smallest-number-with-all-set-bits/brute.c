/* Slow and obviously right: try n, n + 1, n + 2, ... and stop at the
 * first number whose binary digits, read off one by one, are all 1. */
static int all_ones(int x)
{
    while (x > 0) {
        if (x % 2 == 0)
            return 0;
        x /= 2;
    }
    return 1;
}

int smallestNumber(int n)
{
    int x = n;

    while (!all_ones(x))
        x++;
    return x;
}
