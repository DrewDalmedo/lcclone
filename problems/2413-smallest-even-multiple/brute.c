/* Slow and obviously right: walk the list n, 2n, 3n, ... and stop at the
 * first even entry. */
int smallestEvenMultiple(int n)
{
    int m = n;

    while (m % 2 != 0)
        m += n;
    return m;
}
