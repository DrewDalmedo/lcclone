/* Reference solution: an even n is already even, so it comes first on
 * its own list; for an odd n, n is odd and 2 * n is the next entry. */
int smallestEvenMultiple(int n)
{
    return n % 2 == 0 ? n : 2 * n;
}
